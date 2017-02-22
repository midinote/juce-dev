#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class MainContentComponent   : public AudioAppComponent,
                               public Slider::Listener
{
public:
    MainContentComponent()
    :   currentSampleRate (0.0),
        currentAngle (0.0),
        angleDelta (0.0),
        currentFrequency (500.0),
        targetFrequency (currentFrequency),
        currentLevel (0.125),
        targetLevel (currentLevel),
        pan (0.0)
    {
        addAndMakeVisible (frequencySlider);
        frequencySlider.setRange (50.0, 5000.0);
        frequencySlider.setSkewFactorFromMidPoint (500.0); // [4]
        frequencySlider.setValue(500.0);
        frequencySlider.addListener (this);
        
        addAndMakeVisible(levelSlider);
        levelSlider.setRange (0.0, 1.0);
        levelSlider.setValue(0.125);
        levelSlider.addListener(this);
        
        addAndMakeVisible(panSlider);
        panSlider.setRange(-0.5, 0.5);
        panSlider.setValue(0.0);
        panSlider.addListener(this);
        
        setSize (600, 100);
        setAudioChannels (0, 2); // no inputs, one output
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    void resized() override
    {
        frequencySlider.setBounds (10, 10, getWidth() - 20, 20);
        levelSlider.setBounds (10, 20, getWidth() - 20, 40);
        panSlider.setBounds (10, 30, getWidth() - 20, 60);
    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &frequencySlider)
            targetFrequency = frequencySlider.getValue();
        else if (slider == &levelSlider)
            targetLevel = levelSlider.getValue();
        else if (slider == &panSlider)
            pan = panSlider.getValue();
    }
    
    void updateAngleDelta()
    {
        const double cyclesPerSample = frequencySlider.getValue() / currentSampleRate; // [2]
        angleDelta = cyclesPerSample * 2.0 * double_Pi;                                // [3]
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        currentSampleRate = sampleRate;
        updateAngleDelta();
    }
    
    void releaseResources() override
    {
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        float* const bufferL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        float* const bufferR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        const float localTargetLevel = targetLevel;
        const double localTargetFrequency = targetFrequency;
        
        if ((localTargetFrequency != currentFrequency) && (localTargetLevel == currentLevel))
        {
            const double frequencyIncrement = (localTargetFrequency - currentFrequency) / bufferToFill.numSamples;
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                const float currentSample = (float) std::sin(currentAngle);
                currentFrequency += frequencyIncrement;
                updateAngleDelta();
                currentAngle += angleDelta;
                bufferL[sample] = currentSample * currentLevel * (0.5 - pan);
                bufferR[sample] = currentSample * currentLevel * (0.5 + pan);

            }
            
            currentFrequency = localTargetFrequency;
        }
        else if ((localTargetFrequency == currentFrequency) && (localTargetLevel == currentLevel))
        {
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                const float currentSample = (float) std::sin (currentAngle);
                currentAngle += angleDelta;
                bufferL[sample] = currentSample * currentLevel * (0.5 - pan);
                bufferR[sample] = currentSample * currentLevel * (0.5 + pan);
            }
        }
        else if ((localTargetFrequency != currentFrequency) && (localTargetLevel != currentLevel))
        {
            const double frequencyIncrement = (localTargetFrequency - currentFrequency) / bufferToFill.numSamples;
            const double levelIncrement = (localTargetLevel - currentLevel) / bufferToFill.numSamples;
            
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                const float currentSample = (float) std::sin(currentAngle);
                currentFrequency += frequencyIncrement;
                currentLevel += levelIncrement;
                updateAngleDelta();
                currentAngle += angleDelta;
                bufferL[sample] = currentSample * currentLevel * (0.5 - pan);
                bufferR[sample] = currentSample * currentLevel * (0.5 + pan);
                
            }
            
            currentFrequency = localTargetFrequency;
            currentLevel = localTargetLevel;
        }
        else
        {
            const double levelIncrement = (localTargetLevel - currentLevel) / bufferToFill.numSamples;
            
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                const float currentSample = (float) std::sin (currentAngle);
                currentLevel += levelIncrement;
                currentAngle += angleDelta;
                bufferL[sample] = currentSample * currentLevel * (0.5 - pan);
                bufferR[sample] = currentSample * currentLevel * (0.5 + pan);
            }
            
            currentLevel = localTargetLevel;
        }
    }
    
    
private:
    Slider frequencySlider;
    Slider levelSlider;
    Slider panSlider;
    double currentSampleRate, currentAngle, angleDelta, currentFrequency, targetFrequency,
        currentLevel, targetLevel, pan; // [1]
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
