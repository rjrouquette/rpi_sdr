//
// Created by robert on 3/11/17.
//

#ifndef KMOD_ADC_SDR_ADC_H
#define KMOD_ADC_SDR_ADC_H

#include <inttypes.h>

#define SAMPLE_BUFFER_LENGTH (51200000) // 1 second at 51.2 MS/s

typedef struct sdr_adc_s {
    uint32_t pps_offset;
    uint32_t update_position;
    char sampleBuffer[SAMPLE_BUFFER_LENGTH];
} sdr_adc_t;

#endif //KMOD_ADC_SDR_ADC_H
