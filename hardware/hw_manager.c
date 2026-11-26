    USART3_Init();
    ADC1_DMA_stream_init(adc_buf, ADC_FULL_BUFFER);
    
    while (1) {
        if (DMA_HALF_Poll()) {
            USART3_SendBytes(SYNC_HDR, 4U);
            USART3_SendBytes((const uint8_t *)&adc_buf[0], ADC_HALF_BUFFER * 2U);
        }
        if (DMA_FULL_Poll()) {
            USART3_SendBytes(SYNC_HDR, 4U);
            USART3_SendBytes((const uint8_t *)&adc_buf[ADC_HALF_BUFFER], ADC_HALF_BUFFER * 2U);
        }
    }

