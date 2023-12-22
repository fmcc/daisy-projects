#pragma once
#ifndef DSY_PETAL_H
#define DSY_PETAL_H /**< & */

#include "daisy_seed.h"

namespace terrarium
{
/**
   @brief Helpers and hardware definitions for terrarium. 
   @ingroup boards
*/
class Terrarium
{
  public:
    enum Sw
    {
        SW_1, 
        SW_2, 
        SW_3, 
        SW_4, 
        SW_LAST
    }; 
    enum Ftsw
    {
        FTSW_1, 
        FTSW_2,
        FTSW_LAST
    }; 
    enum Pot
    {
        POT_1, 
        POT_2, 
        POT_3, 
        POT_4, 
        POT_5, 
        POT_6, 
        POT_LAST
    }; 
    enum FtswLed
    {
        LED_1, 
        LED_2
        LED_LAST
    }; 
    /** Constructor */
    Terrarium() {}
    /** Destructor */
    ~Terrarium() {}

    /** Initialize terrarium */
    void Init(bool boost = false);

    /**
       Wait before moving on.
       \param del Delay time in ms.
     */
    void DelayMs(size_t del);


    /** Starts the callback
    \param cb Interleaved callback function
    */
    void StartAudio(AudioHandle::InterleavingAudioCallback cb);

    /** Starts the callback
    \param cb multichannel callback function
    */
    void StartAudio(AudioHandle::AudioCallback cb);

    /**
       Switch callback functions
       \param cb New interleaved callback function.
    */
    void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);

    /**
       Switch callback functions
       \param cb New multichannel callback function.
    */
    void ChangeAudioCallback(AudioHandle::AudioCallback cb);

    /** Stops the audio if it is running. */
    void StopAudio();

    /** Updates the Audio Sample Rate, and reinitializes.
     ** Audio must be stopped for this to work.
     */
    void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);

    /** Returns the audio sample rate in Hz as a floating point number.
     */
    float AudioSampleRate();

    /** Sets the number of samples processed per channel by the audio callback.
       \param size Audio block size
     */
    void SetAudioBlockSize(size_t size);

    /** Returns the number of samples per channel in a block of audio. */
    size_t AudioBlockSize();

    /** Returns the rate in Hz that the Audio callback is called */
    float AudioCallbackRate();

    /** Start analog to digital conversion. */
    void StartAdc();

    /** Stops Transfering data from the ADC */
    void StopAdc();

    /** Call at the same frequency as controls are read for stable readings.*/
    void ProcessAnalogControls();

    /** Process Analog and Digital Controls */
    inline void ProcessAllControls()
    {
        ProcessAnalogControls();
        ProcessDigitalControls();
    }


    /** Get value per knob.
    \param k Which knob to get
    \return Floating point knob position.
    */
    float GetKnobValue(Knob k);

    /** & */
    float GetExpression();

    /** Process digital controls */
    void ProcessDigitalControls();

    /** Turn all leds off */
    void ClearLeds();

    /** Update Leds to values you had set. */
    void UpdateLeds();

    void SetLed(Led idx, float bright);

    DaisySeed seed;    /**< & */

    AnalogControl pots[POT_LAST]; /**< & */
    Switch        switches[SW_LAST] /**< & */;
    Switch        footswitches[FTSW_LAST] /**< & */;
    Led    leds[LED_LAST]; /**< & */

  private:
    void SetHidUpdateRates();
    void InitSwitches();
    void InitLeds();
    void InitAnalogControls();

    inline uint16_t* adc_ptr(const uint8_t chn) { return seed.adc.GetPtr(chn); }

};

} // namespace terrarium

#endif
