#include "hUsart.h"

// Внутренний регистр для отслеживания состояний usart+edma смотри UartVirRegBits
uint32_t UartVirtStateRegister = 0;
#if STATIC_MEM == 1
__attribute__((aligned(16))) hEdmaDescriptors_t EdmaDescriptors[EDMA_COUNT_STREAM][DESCRIPTORS_SIZE_BUF] = {0};
#else
__attribute__((aligned(16))) hEdmaDescriptors_t *EdmaDescriptors[EDMA_COUNT_STREAM];
#endif
/*-------------------Внутренние функции библиотеки*/
int UsartClockEn(usart_type *usart_x);
int EdmaCheckDescriptors(void);
int UsartEdmaSetDescriptors(edma_stream_type *edma_streamx, hEdmaDescriptors_t *descriptors_x);
int FindCountDescriptors(edma_stream_type *edma_streamx);
edmamux_requst_id_sel_type EdmaFindIdEDMUX(usart_type *usart_x);
/*-------------Внутренние функции библиотеки //end*/

/*!
 * Функция включает тактирования usart и устанавливает статусные биты во внутреннем регистре.
 * @param[in] usart_x адрес переферии usart
 * @return 0 - succsess else !0
 */
int UsartClockEn(usart_type *usart_x)
{
    int ret = 1;
    if (usart_x == USART1)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << USART1_EN_CLOCK);
        crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
    }
    else if (usart_x == USART2)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << USART2_EN_CLOCK);
        crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
    }
    else if (usart_x == USART3)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << USART3_EN_CLOCK);
        crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE);
    }
    else if (usart_x == UART4)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << UART4_EN_CLOCK);
        crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, TRUE);
    }
    else if (usart_x == UART5)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << UART5_EN_CLOCK);
        crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, TRUE);
    }
    else if (usart_x == USART6)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << USART6_EN_CLOCK);
        crm_periph_clock_enable(CRM_USART6_PERIPH_CLOCK, TRUE);
    }
    else if (usart_x == UART7)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << UART7_EN_CLOCK);
        crm_periph_clock_enable(CRM_UART7_PERIPH_CLOCK, TRUE);
    }
    if (ret != 0)
    {
        UartVirtStateRegister |= USART_ERR;
    }
    return ret;
}

/*!
 * Функция инициалицазии usart
 * @param[in] hand_x параметры обработчика для Usart.
 * @return 0 - succsess else !0
 * @todo не включает прерывания, не настраивает gpio
 */
int hUsartInit(hUsartHand_t *hand_x)
{
    int ret = 0;
    if ((ret = UsartClockEn(hand_x->usart_x)) != 0)
    {
        return 1;
    }
    usart_init(hand_x->usart_x, hand_x->param.baud_rate, hand_x->param.data_bit, hand_x->param.stop_bit);
    usart_transmitter_enable(hand_x->usart_x, TRUE);
    usart_dma_transmitter_enable(hand_x->usart_x, TRUE);
    usart_enable(hand_x->usart_x, TRUE);
    return 0;
}
/*!
 * Функция для отправки байта
 * @param[in] usart_x адрес переферии usart
 * @param[in] byte байт данных
 */
void hUsartTransmitByte(usart_type *usart_x, uint8_t byte)
{
    usart_x->dt = byte;
}
/*!
 * Функция для отправки байт в блокирующем режиме
 * @param[in] usart_x адрес переферии usart
 * @param[in] data указатель на данные
 * @param[in] DataSize количество байт для отправки
 */
void hUsartTransmitData(usart_type *usart_x, uint8_t *data, uint32_t DataSize)
{
    while (DataSize)
    {
        hUsartTransmitByte(usart_x, *data);
        while (usart_flag_get(usart_x, USART_TDBE_FLAG) == RESET)
            ;
        data++;
        DataSize--;
    }
}
/*!
 * Функция для поиска дескриптора EDMA
 * @param[in] edma_streamx адрес переферии edma
 * @return возвращает номер дескриптора в EdmaDescriptors[] else -1 в случае ошибки
 */
int FindCountDescriptors(edma_stream_type *edma_streamx)
{
    int NumDescriptors = -1;
    if (edma_streamx == EDMA_STREAM1)
    {
        NumDescriptors = 0;
    }
    else if (edma_streamx == EDMA_STREAM2)
    {
        NumDescriptors = 1;
    }
    else if (edma_streamx == EDMA_STREAM3)
    {
        NumDescriptors = 2;
    }
    else if (edma_streamx == EDMA_STREAM4)
    {
        NumDescriptors = 3;
    }
    else if (edma_streamx == EDMA_STREAM5)
    {
        NumDescriptors = 4;
    }
    else if (edma_streamx == EDMA_STREAM6)
    {
        NumDescriptors = 5;
    }
    else if (edma_streamx == EDMA_STREAM7)
    {
        NumDescriptors = 6;
    }
    else if (edma_streamx == EDMA_STREAM8)
    {
        NumDescriptors = 7;
    }
    return NumDescriptors;
}

#if STATIC_MEM == 1

/*!
 * Функция для работы со статически выделенной памятью добавляет данные для дескриптора edma
 * @param[in] edma_streamx адрес переферии edma
 * @param[in] EdmaDesData данные для отпавки
 * @return 0 - succsess else !0
 * @todo нет проверки на принадлежность Usart и Дескриптора
 */
int hUsartEdmaAddDescriptors(hEdmaDescriptorData_t *EdmaDesData, edma_stream_type *edma_streamx)
{
    int NumDescriptor = FindCountDescriptors(edma_streamx);
    if (NumDescriptor < 0)
    {
        return 1;
    }

    static uint32_t count = 0;
    EdmaDescriptors[NumDescriptor][count].ctrl_dtcnt = (0x20090000 | EdmaDesData->bufSize);
    EdmaDescriptors[NumDescriptor][count].m0addr = (uint32_t)&EdmaDesData->ptrBuf[0];
    EdmaDescriptors[NumDescriptor][count].paddr = (uint32_t)(&(EdmaDesData->usart_x->dt));
    EdmaDescriptors[NumDescriptor][count].nextLLp = (uint32_t)&EdmaDescriptors[NumDescriptor][count + 1];
    count++;
    return 0;
}
#else
/*!
 * Функция для работы с динамически выделенной памятью добавляет данные для дескриптора edma
 * @param[in] edma_streamx адрес переферии edma
 * @param[in] EdmaDesData данные для отпавки
 * @return 0 - succsess else !0
 * @todo нет проверки на принадлежность Usart и Дескриптора
 */
int hUsartEdmaAddDescriptors(hEdmaDescriptorData_t *EdmaDesData, edma_stream_type *edma_streamx)
{
    int NumDescriptors = FindCountDescriptors(edma_streamx);
    if (NumDescriptors < 0)
    {
        return 1;
    }
    hEdmaDescriptors_t *NewEdmaDescriptors = (hEdmaDescriptors_t *)memalign(sizeof(hEdmaDescriptors_t), 16);
    if (NewEdmaDescriptors == NULL)
    {
        return 1;
    }
    NewEdmaDescriptors->ctrl_dtcnt = (0x20090000 | EdmaDesData->bufSize);
    NewEdmaDescriptors->m0addr = (uint32_t)&EdmaDesData->ptrBuf[0];
    NewEdmaDescriptors->paddr = (uint32_t)(&(EdmaDesData->usart_x->dt));
    NewEdmaDescriptors->nextLLp = (uint32_t)EdmaDescriptors[NumDescriptors];
    EdmaDescriptors[NumDescriptors] = NewEdmaDescriptors;
    return 0;
}
#endif

/*!
 * Функция для включения linked table transfer edma
 * @param[in] edma_streamx адрес переферии edma
 * @param[in] descriptors_x таблица дескрипторов с данными для отправки
 * @return 0 - succsess else !0
 */
int UsartEdmaSetDescriptors(edma_stream_type *edma_streamx, hEdmaDescriptors_t *descriptors_x)
{
    int ret = 1;
    if (edma_streamx == EDMA_STREAM1)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM1_EN);
        edma_link_list_enable(EDMA_STREAM1_LL, TRUE);
        edma_link_list_init(EDMA_STREAM1_LL, (uint32_t)descriptors_x);
    }
    else if (edma_streamx == EDMA_STREAM2)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM2_EN);
        edma_link_list_enable(EDMA_STREAM2_LL, TRUE);
        edma_link_list_init(EDMA_STREAM2_LL, (uint32_t)descriptors_x);
    }
    else if (edma_streamx == EDMA_STREAM3)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM3_EN);
        edma_link_list_enable(EDMA_STREAM3_LL, TRUE);
        edma_link_list_init(EDMA_STREAM3_LL, (uint32_t)descriptors_x);
    }
    else if (edma_streamx == EDMA_STREAM4)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM4_EN);
        edma_link_list_enable(EDMA_STREAM4_LL, TRUE);
        edma_link_list_init(EDMA_STREAM4_LL, (uint32_t)descriptors_x);
    }
    else if (edma_streamx == EDMA_STREAM5)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM5_EN);
        edma_link_list_enable(EDMA_STREAM5_LL, TRUE);
        edma_link_list_init(EDMA_STREAM5_LL, (uint32_t)descriptors_x);
    }
    else if (edma_streamx == EDMA_STREAM6)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM6_EN);
        edma_link_list_enable(EDMA_STREAM6_LL, TRUE);
        edma_link_list_init(EDMA_STREAM6_LL, (uint32_t)descriptors_x);
    }
    else if (edma_streamx == EDMA_STREAM7)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM7_EN);
        edma_link_list_enable(EDMA_STREAM7_LL, TRUE);
        edma_link_list_init(EDMA_STREAM7_LL, (uint32_t)descriptors_x);
    }
    else if (edma_streamx == EDMA_STREAM8)
    {
        ret = 0;
        UartVirtStateRegister |= (1 << EDMA_STREAM8_EN);
        edma_link_list_enable(EDMA_STREAM8_LL, TRUE);
        edma_link_list_init(EDMA_STREAM8_LL, (uint32_t)descriptors_x);
    }
    if (ret != 0)
    {
        UartVirtStateRegister |= EDMA_ERR;
    }
    return ret;
}

/*!
 * Функция поиска id для коммутации MUX
 * @param[in] usart_x адрес переферии usart
 * @return edmamux channel request inputs resources: usartN_tx или 0 если usart выбран не корректно
 */
edmamux_requst_id_sel_type EdmaFindIdEDMUX(usart_type *usart_x)
{
    if (usart_x == USART1)
    {
        return EDMAMUX_DMAREQ_ID_USART1_TX;
    }
    else if (usart_x == USART2)
    {
        return EDMAMUX_DMAREQ_ID_USART2_TX;
    }
    else if (usart_x == USART3)
    {
        return EDMAMUX_DMAREQ_ID_USART3_TX;
    }
    else if (usart_x == UART4)
    {
        return EDMAMUX_DMAREQ_ID_UART4_TX;
    }
    else if (usart_x == UART5)
    {
        return EDMAMUX_DMAREQ_ID_UART5_TX;
    }
    else if (usart_x == USART6)
    {
        return EDMAMUX_DMAREQ_ID_USART6_TX;
    }
    else if (usart_x == UART7)
    {
        return EDMAMUX_DMAREQ_ID_UART7_TX;
    }
    return 0;
}
/*!
 * Функция инициализирует Usart для отправки данных в режиме edma EDMA linked table transfer.
 * linked table transfer позволяет формировать данные для отправки из различных адресов памяти edma соберёт буфер в автоматическом режиме.
 * @param[in] hand_x параметры обработчика для Usart+edma.
 * @return 0 - succsess else !0
 * @todo функция не настраивает Usart, Дескрипторы с данными настраиваютя отдлено; должны быть настроины до вызова данной функции
*/
int hUsartEdmaInit(hEdmaUsartHand_t *hand_x)
{

    int NumDescriptors = FindCountDescriptors(hand_x->edma_streamx);
    if (NumDescriptors < 0)
    {
        return 1;
    }

#if STATIC_MEM == 1
    hEdmaDescriptors_t *tmpAddressDescriptor = (hEdmaDescriptors_t *)&EdmaDescriptors[NumDescriptors][0];
#else
    hEdmaDescriptors_t *tmpAddressDescriptor = (hEdmaDescriptors_t *)EdmaDescriptors[NumDescriptors];
#endif
    crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

    if (UsartEdmaSetDescriptors(hand_x->edma_streamx, tmpAddressDescriptor) != 0)
    {
        return 1;
    }

    edmamux_enable(TRUE);
    edmamux_requst_id_sel_type id = EdmaFindIdEDMUX(hand_x->usart_x);
    edmamux_init(hand_x->edmamux_channelx, id);

    edma_interrupt_enable(hand_x->edma_streamx, EDMA_FDT_INT, TRUE);
    nvic_irq_enable(EDMA_Stream1_IRQn, EDMA_NVIC_PRIOR, EDMA_NVIC_SUB_PRIOR);
    return 0;
}

/*!
 * Функция для старта передачи данных из ранее установленных дескрипторов
 * @param[in] hand_x параметры обработчика для Usart+edma.
 * @todo при множественном вызове не будет успевать снять флаг edma_flag_clear(EDMA_FDT1_FLAG);
 * часть посылок может потеряться
 */
void hUsartEdmaStartTransmit(hEdmaUsartHand_t *hand_x)
{
    edma_stream_enable(hand_x->edma_streamx, TRUE);
}

// TODO: необходимо сделать EDMA_STREAM_CallBack(edma_stream_type *edma_streamx, uint32_t event)
// для всех потоков edma_stream_type. Необходимо вынести данное в отдельную библиотеку halIRQ.h
void EDMA_Stream1_IRQHandler(void)
{
    if (edma_flag_get(EDMA_FDT1_FLAG) != RESET)
    {
        edma_flag_clear(EDMA_FDT1_FLAG);
    }
}

#if STATIC_MEM == 1
/*!
 * Функция для проверки корректной настройки Дескриптора со статически выделенной памятью
 * @param[in] edma_streamx параметры обработчика для edma.
 * @todo printf должен быть настроен до проверки
 */
void PrintList(edma_stream_type *edma_streamx)
{
    int NumDescriptor = FindCountDescriptors(edma_streamx);
    if (NumDescriptor < 0)
    {
        return;
    }

    for (uint16_t i = 0; i < DESCRIPTORS_SIZE_BUF; i++)
    {
        printf("arr::0x%lx\t0x%lx\t0x%lx\t0x%8lx\t\t%p\t%d\r\n", EdmaDescriptors[NumDescriptor][i].ctrl_dtcnt, EdmaDescriptors[NumDescriptor][i].m0addr, EdmaDescriptors[NumDescriptor][i].paddr, EdmaDescriptors[NumDescriptor][i].nextLLp, &EdmaDescriptors[NumDescriptor][i], sizeof(EdmaDescriptors[NumDescriptor][i]));
        char *tmp = (char *)EdmaDescriptors[NumDescriptor][i].m0addr;
        uint16_t tmpCount = EdmaDescriptors[NumDescriptor][i].ctrl_dtcnt;
        for (uint16_t j = 0; j < tmpCount; j++)
        {
            printf("%c ", tmp[j]);
        }
        printf("\r\n");
    }
}
#else
/*!
 * Функция для проверки корректной настройки Дескриптора с динамически выделенной памятью
 * @param[in] edma_streamx параметры обработчика для edma.
 * @todo printf должен быть настроен до проверки
 */
void PrintList(edma_stream_type *edma_streamx)
{
    int NumDescriptors = FindCountDescriptors(edma_streamx);
    if (NumDescriptors < 0)
    {
        return;
    }

    hEdmaDescriptors_t *head = EdmaDescriptors[NumDescriptors];
    while (head)
    {
        printf("head::0x%lx\t0x%lx\t0x%lx\t0x%8lx\t\t%p\t%d\r\n", head->ctrl_dtcnt, head->m0addr, head->paddr, head->nextLLp, head, sizeof(*head));
        char *tmp = (char *)head->m0addr;
        uint16_t tmpCount = head->ctrl_dtcnt;
        for (size_t i = 0; i < tmpCount; i++)
        {
            printf("%c ", tmp[i]);
        }
        printf("\r\n");
        head = (hEdmaDescriptors_t *)head->nextLLp;
    }
}
#endif