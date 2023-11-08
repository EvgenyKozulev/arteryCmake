#ifndef __Main_h__
#define __Main_h__

#include "at32f435_437_clock.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include <stdio.h>
#include "hUsart/hUsart.h"

#include "lwipopts.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "ethernetif.h"
#include "at32_emac.h"
#include "lwip/netif.h"
#include "hNet/netconf.h"
#include "hNet/tcp_client.h"

#endif //__Main_h__