#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef __APPLE__
#define strdupa strdup
#include <libgen.h>
#else
#include <stdio_ext.h>
#endif

#include "include/version.h"
#include "include/ieee80211.h"
#include "include/hcxpcaptool.h"
#include "include/common.c"
#include "include/byteops.c"
#include "include/fileops.c"
#include "include/pcap.c"

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define BIG_ENDIAN_HOST
#endif

/*===========================================================================*/
/* global var */

bool hexmodeflag;

int apstaessidcount;
apstaessidl_t *apstaessidliste;

unsigned long long int beaconframecount;
unsigned long long int proberequestframecount;
unsigned long long int proberesponseframecount;
unsigned long long int associationrequestframecount;
unsigned long long int associationresponseframecount;
unsigned long long int reassociationrequestframecount;
unsigned long long int reassociationresponseframecount;

char *hexmodeoutname;
char *essidoutname;
char *trafficoutname;

FILE *fhhexmode;
/*===========================================================================*/
/* global init */

bool globalinit()
{
hexmodeoutname = NULL;
essidoutname = NULL;
trafficoutname = NULL;

hexmodeflag = false;

setbuf(stdout, NULL);
srand(time(NULL));
return true;
}
/*===========================================================================*/
char *getdltstring(int networktype) 
{
switch(networktype)
	{
	case DLT_NULL: return "DLT_NULL";
	case DLT_EN10MB: return "DLT_EN10MB";
	case DLT_AX25: return "DLT_AX25";
	case DLT_IEEE802: return "DLT_IEEE802";
	case DLT_ARCNET: return "DLT_ARCNET";
	case DLT_SLIP: return "DLT_SLIP";
	case DLT_PPP: return "DLT_PPP";
	case DLT_FDDI: return "DLT_FDDI";
	case DLT_PPP_SERIAL: return "DLT_PPP_SERIAL";
	case DLT_PPP_ETHER: return "DLT_PPP_ETHER";
	case DLT_ATM_RFC1483: return "DLT_ATM_RFC1483";
	case DLT_RAW: return "DLT_RAW";
	case DLT_C_HDLC: return "DLT_C_HDLC";
	case DLT_IEEE802_11: return "DLT_IEEE802_11";
	case DLT_FRELAY: return "DLT_FRELAY";
	case DLT_LOOP: return "DLT_LOOP";
	case DLT_LINUX_SLL: return "DLT_LINUX_SLL";
	case DLT_LTALK: return "DLT_LTALK";
	case DLT_PFLOG: return "DLT_PFLOG";
	case DLT_PRISM_HEADER: return "DLT_PRISM_HEADER";
	case DLT_IP_OVER_FC: return "DLT_IP_OVER_FC";
	case DLT_SUNATM: return "DLT_SUNATM";
	case DLT_IEEE802_11_RADIO: return "DLT_IEEE802_11_RADIO";
	case DLT_ARCNET_LINUX: return "DLT_ARCNET_LINUX";
	case DLT_APPLE_IP_OVER_IEEE1394: return "DLT_APPLE_IP_OVER_IEEE1394";
	case DLT_MTP2_WITH_PHDR: return "DLT_MTP2_WITH_PHDR";
	case DLT_MTP2: return "DLT_MTP2";
	case DLT_MTP3: return "DLT_MTP3";
	case DLT_SCCP: return "DLT_SCCP";
	case DLT_DOCSIS: return "DLT_DOCSIS";
	case DLT_LINUX_IRDA: return "DLT_LINUX_IRDA";
	case 147 :return "DLT_USER0-DLT_USER15";
	case 148:return "DLT_USER0-DLT_USER15";
	case 149:return "DLT_USER0-DLT_USER15";
	case 150:return "DLT_USER0-DLT_USER15";
	case 151:return "DLT_USER0-DLT_USER15";
	case 152:return "DLT_USER0-DLT_USER15";
	case 153:return "DLT_USER0-DLT_USER15";
	case 154:return "DLT_USER0-DLT_USER15";
	case 155:return "DLT_USER0-DLT_USER15";
	case 156:return "DLT_USER0-DLT_USER15";
	case 157:return "DLT_USER0-DLT_USER15";
	case 158:return "DLT_USER0-DLT_USER15";
	case 159:return "DLT_USER0-DLT_USER15";
	case 160:return "DLT_USER0-DLT_USER15";
	case 161:return "DLT_USER0-DLT_USER15";
	case 162:return "DLT_USER0-DLT_USER15";
	case DLT_IEEE802_11_RADIO_AVS: return "DLT_IEEE802_11_RADIO_AVS";
	case DLT_BACNET_MS_TP: return "DLT_BACNET_MS_TP";
	case DLT_PPP_PPPD: return "DLT_PPP_PPPD";
	case DLT_GPRS_LLC: return "DLT_GPRS_LLC";
	case DLT_GPF_T: return "DLT_GPF_T";
	case DLT_GPF_F: return "DLT_GPF_F";
	case DLT_LINUX_LAPD: return "DLT_LINUX_LAPD";
	case DLT_BLUETOOTH_HCI_H4: return "DLT_BLUETOOTH_HCI_H4";
	case DLT_USB_LINUX: return "DLT_USB_LINUX";
	case DLT_PPI: return "DLT_PPI";
	case DLT_IEEE802_15_4: return "DLT_IEEE802_15_4";
	case DLT_SITA: return "DLT_SITA";
	case DLT_ERF: return "DLT_ERF";
	case DLT_BLUETOOTH_HCI_H4_WITH_PHDR: return "DLT_BLUETOOTH_HCI_H4_WITH_PHDR";
	case DLT_AX25_KISS: return "DLT_AX25_KISS";
	case DLT_LAPD: return "DLT_LAPD";
	case DLT_PPP_WITH_DIR: return "DLT_PPP_WITH_DIR";
	case DLT_C_HDLC_WITH_DIR: return "DLT_C_HDLC_WITH_DIR";
	case DLT_FRELAY_WITH_DIR: return "DLT_FRELAY_WITH_DIR";
	case DLT_IPMB_LINUX: return "DLT_IPMB_LINUX";
	case DLT_IEEE802_15_4_NONASK_PHY: return "DLT_IEEE802_15_4_NONASK_PHY";
	case DLT_USB_LINUX_MMAPPED: return "DLT_USB_LINUX_MMAPPED";
	case DLT_FC_2: return "DLT_FC_2";
	case DLT_FC_2_WITH_FRAME_DELIMS: return "DLT_FC_2_WITH_FRAME_DELIMS";
	case DLT_IPNET: return "DLT_IPNET";
	case DLT_CAN_SOCKETCAN: return "DLT_CAN_SOCKETCAN";
	case DLT_IPV4: return "DLT_IPV4";
	case DLT_IPV6: return "DLT_IPV6";
	case DLT_IEEE802_15_4_NOFCS: return "DLT_IEEE802_15_4_NOFCS";
	case DLT_DBUS: return "DLT_DBUS";
	case DLT_DVB_CI: return "DLT_DVB_CI";
	case DLT_MUX27010: return "DLT_MUX27010";
	case DLT_STANAG_5066_D_PDU: return "DLT_STANAG_5066_D_PDU";
	case DLT_NFLOG: return "DLT_NFLOG";
	case DLT_NETANALYZER: return "DLT_NETANALYZER";
	case DLT_NETANALYZER_TRANSPARENT: return "DLT_NETANALYZER_TRANSPARENT";
	case DLT_IPOIB: return "DLT_IPOIB";
	case DLT_MPEG_2_TS: return "DLT_MPEG_2_TS";
	case DLT_NG40: return "DLT_NG40";
	case DLT_NFC_LLCP: return "DLT_NFC_LLCP";
	case DLT_INFINIBAND: return "DLT_INFINIBAND";
	case DLT_SCTP: return "DLT_SCTP";
	case DLT_USBPCAP: return "DLT_USBPCAP";
	case DLT_RTAC_SERIAL: return "DLT_RTAC_SERIAL";
	case DLT_BLUETOOTH_LE_LL: return "DLT_BLUETOOTH_LE_LL";
	case DLT_NETLINK: return "DLT_NETLINK";
	case DLT_BLUETOOTH_LINUX_MONITOR: return "DLT_BLUETOOTH_LINUX_MONITOR";
	case DLT_BLUETOOTH_BREDR_BB: return "DLT_BLUETOOTH_BREDR_BB";
	case DLT_BLUETOOTH_LE_LL_WITH_PHDR: return "DLT_BLUETOOTH_LE_LL_WITH_PHDR";
	case DLT_PROFIBUS_DL: return "DLT_PROFIBUS_DL";
	case DLT_PKTAP: return "DLT_PKTAP";
	case DLT_EPON: return "DLT_EPON";
	case DLT_IPMI_HPM_2: return "DLT_IPMI_HPM_2";
	case DLT_ZWAVE_R1_R2: return "DLT_ZWAVE_R1_R2";
	case DLT_ZWAVE_R3: return "DLT_ZWAVE_R3";
	case DLT_WATTSTOPPER_DLM: return "DLT_WATTSTOPPER_DLM";
	case DLT_ISO_14443: return "DLT_ISO_14443";
	case DLT_RDS: return "DLT_RDS";
	case DLT_USB_DARWIN: return "DLT_USB_DARWIN";
	case DLT_SDLC: return "DLT_SDLC";
	default: return "unknown network type";
	}
return "unknown network type";
}
/*===========================================================================*/
char *geterrorstat(int errorstat) 
{
switch(errorstat)
	{
	case 0: return "flawless";
	case 1: return "yes";
	default: return "unknown";
	}
return "unknown";
}
/*===========================================================================*/
char *getendianessstring(int endianess) 
{
switch(endianess)
	{
	case 0: return "little endian";
	case 1: return "big endian";
	default: return "unknown endian";
	}
return "unknow nendian";
}
/*===========================================================================*/
void printcapstatus(char *pcaptype, char *pcapinname, int version_major, int version_minor, int networktype, int endianess, unsigned long long int rawpacketcount, unsigned long long int skippedpacketcount, int pcapreaderrors, bool tscleanflag)
{
printf("summary:\n--------\n"
	"file name..............: %s\n"
	"file type..............: %s %d.%d\n"
	"network type...........: %s (%d)\n"
	"endianess..............: %s\n"
	"read errors............: %s\n"
	"packets inside.........: %lld\n"
	"skippedpackets.........: %lld\n"
	, basename(pcapinname), pcaptype, version_major, version_minor, getdltstring(networktype), networktype, getendianessstring(endianess), geterrorstat(pcapreaderrors), rawpacketcount, skippedpacketcount);

if(tscleanflag == true)
	{
	printf("warning................: zero value timestamps detected\n");
	}
if(beaconframecount != 0)
	{
	printf("beacons................: %lld\n", beaconframecount);
	}
if(proberequestframecount != 0)
	{
	printf("probe requests.........: %lld\n", proberequestframecount);
	}
if(proberesponseframecount != 0)
	{
	printf("probe responses........: %lld\n", proberesponseframecount);
	}
if(associationrequestframecount != 0)
	{
	printf("association requests...: %lld\n", associationrequestframecount);
	}
if(associationresponseframecount != 0)
	{
	printf("association responses..: %lld\n", associationresponseframecount);
	}
if(reassociationrequestframecount != 0)
	{
	printf("reassociation requests.: %lld\n", reassociationrequestframecount);
	}
if(reassociationresponseframecount != 0)
	{
	printf("reassociation responses: %lld\n", reassociationresponseframecount);
	}

printf("\n");
return;
}
/*===========================================================================*/
void packethexdump(uint32_t ts_sec, uint32_t ts_usec, unsigned long long int packetnr, int networktype, int snaplen, int caplen, int len, uint8_t *packet)
{
int c, d;
time_t pkttime;
struct tm *pkttm;
char tmbuf[64], pcktimestr[64];

pkttime = ts_sec;
pkttm = localtime(&pkttime);
strftime(tmbuf, sizeof tmbuf, "%d.%m.%Y\ntime.......: %H:%M:%S", pkttm);
snprintf(pcktimestr, sizeof(pcktimestr), "%s.%06lu", tmbuf, (long int)ts_usec);

fprintf(fhhexmode, "packet.....: %lld\n"
	"date.......: %s\n"
	"networktype: %s (%d)\n"
	"snaplen....: %d\n"
	"caplen.....: %d\n"
	"len........: %d\n", packetnr, pcktimestr, getdltstring(networktype), networktype, snaplen, caplen, len);

d = 0;
while(d < caplen)
	{
	for(c = 0; c < 16; c++)
		{
		if((d +c) < caplen)
			{
			fprintf(fhhexmode, "%02x ", packet[d +c]);
			}
		else
			{
			fprintf(fhhexmode, "   ");
			}
		
		}
	fprintf(fhhexmode, "    ");
	for(c = 0; c < 16; c++)
		{
		if((d +c < caplen) && (packet[d +c] >= 0x20) && (packet[d +c] < 0x7f))
			{
			fprintf(fhhexmode, "%c", packet[d +c]);
			}
		else if(d +c < caplen)
			{
			fprintf(fhhexmode, ".");
			}
		}
	fprintf(fhhexmode, "\n");
	d += 16;
	}
fprintf(fhhexmode, "\n");
return;
}
/*===========================================================================*/
void outputlists()
{
int c, p;
FILE *fhoutlist = NULL;
apstaessidl_t *zeiger, *zeigerold;
time_t pkttime;
struct tm *pkttm;

char tmbuf[64];
uint8_t essidstring[34];

if((apstaessidliste != NULL) && (essidoutname != NULL)) 
	{
	if((fhoutlist = fopen(essidoutname, "a+")) != NULL)
		{
		zeiger = apstaessidliste;
		qsort(apstaessidliste, apstaessidcount, APSTAESSIDLIST_SIZE, sort_apstaessidlist_by_essid);
		memset(&essidstring, 0, 34);
		memcpy(&essidstring,  zeiger->essid, 32);
		if(isasciistring(zeiger->essidlen, essidstring) != false)
			{
			fprintf(fhoutlist, "%s\n", essidstring);
			}
		else
			{
			fprintf(fhoutlist, "$HEX[");
			for(p = 0; p < zeiger->essidlen; p++)
				{
				fprintf(fhoutlist, "%02x", essidstring[p]);
				}
			fprintf(fhoutlist, "]\n");
			}
		zeiger++;
		for(c = 1; c < apstaessidcount; c++)
			{
			if(memcmp(&essidstring, zeiger->essid, 32) != 0)
				{
				memset(&essidstring, 0, 34);
				memcpy(&essidstring,  zeiger->essid, 32);
				if(isasciistring(zeiger->essidlen, essidstring) != false)
					{
					fprintf(fhoutlist, "%s\n", essidstring);
					}
				else
					{
					fprintf(fhoutlist, "$HEX[");
					for(p = 0; p < zeiger->essidlen; p++)
						{
						fprintf(fhoutlist, "%02x", essidstring[p]);
						}
					fprintf(fhoutlist, "]\n");
					}
				}
			zeiger++;
			}
		}
	fclose(fhoutlist);
	}

if((apstaessidliste != NULL) && (trafficoutname != NULL))
	{
	if((fhoutlist = fopen(trafficoutname, "a+")) != NULL)
		{
		zeiger = apstaessidliste;
		zeigerold = apstaessidliste;
		qsort(apstaessidliste, apstaessidcount, APSTAESSIDLIST_SIZE, sort_apstaessidlist_by_ap);
		memset(&essidstring, 0, 34);
		memcpy(&essidstring,  zeiger->essid, 32);
		if(zeiger->tv_sec != 0)
			{
			pkttime = zeiger->tv_sec;
			pkttm = localtime(&pkttime);
			strftime(tmbuf, sizeof tmbuf, "%d%m%Y", pkttm);
			fprintf(fhoutlist, "%s:", tmbuf);
			}
		else
			{
			fprintf(fhoutlist, "00000000:");
			}
		for(p = 0; p< 6; p++)
			{
			fprintf(fhoutlist, "%02x", zeiger->mac_sta[p]);
			}
		fprintf(fhoutlist, ":");
		for(p = 0; p< 6; p++)
			{
			fprintf(fhoutlist, "%02x", zeiger->mac_ap[p]);
			}
		fprintf(fhoutlist, ":");
		if(isasciistring(zeiger->essidlen, essidstring) != false)
			{
			fprintf(fhoutlist, "%s\n", essidstring);
			}
		else
			{
			fprintf(fhoutlist, "$HEX[");
			for(p = 0; p < zeiger->essidlen; p++)
				{
				fprintf(fhoutlist, "%02x", essidstring[p]);
				}
			fprintf(fhoutlist, "]\n");
			}
		zeiger++;

		for(c = 1; c < apstaessidcount; c++)
			{
			if((memcmp(zeigerold->mac_ap, zeiger->mac_ap, 6) != 0) && (memcmp(zeigerold->mac_sta, zeiger->mac_sta, 6) != 0) && (memcmp(&essidstring, zeiger->essid, 32) != 0))
				{
				memset(&essidstring, 0, 34);
				memcpy(&essidstring,  zeiger->essid, 32);
				if(zeiger->tv_sec != 0)
					{
					pkttime = zeiger->tv_sec;
					pkttm = localtime(&pkttime);
					strftime(tmbuf, sizeof tmbuf, "%d%m%Y", pkttm);
					fprintf(fhoutlist, "%s:", tmbuf);
					}
				else
					{
					fprintf(fhoutlist, "00000000:");
					}
				for(p = 0; p< 6; p++)
					{
					fprintf(fhoutlist, "%02x", zeiger->mac_sta[p]);
					}
				fprintf(fhoutlist, ":");
				for(p = 0; p< 6; p++)
					{
					fprintf(fhoutlist, "%02x", zeiger->mac_ap[p]);
					}
				fprintf(fhoutlist, ":");
				if(zeiger->essidlen > 32)
					printf("warning\n");

				if(isasciistring(zeiger->essidlen, essidstring) != false)
					{
					fprintf(fhoutlist, "%s\n", essidstring);
					}
				else
					{
					fprintf(fhoutlist, "$HEX[");
					for(p = 0; p < zeiger->essidlen; p++)
						{
						fprintf(fhoutlist, "%02x", essidstring[p]);
						}
					fprintf(fhoutlist, "]\n");
					}
				}
			zeigerold = zeiger;
			zeiger++;
			}
		}
	fclose(fhoutlist);
	}

return;
}
/*===========================================================================*/
void addapstaessid(uint32_t tv_sec, uint8_t *mac_sta, uint8_t *mac_ap, uint8_t essidlen, uint8_t *essid)
{
apstaessidl_t *zeiger, *tmp;
int c;

zeiger = apstaessidliste;
for(c = 0; c < apstaessidcount; c++)
	{
	if((memcmp(mac_ap, zeiger->mac_ap, 6) == 0) && (memcmp(mac_sta, zeiger->mac_sta, 6) == 0) && (memcmp(essid, zeiger->essid, 32) == 0) && (essidlen == zeiger->essidlen))
		{
		if(zeiger->tv_sec == 0)
			{
			zeiger->tv_sec = tv_sec;
			}
		return;
		}
	zeiger++;
	}

zeiger->tv_sec = tv_sec;
memcpy(zeiger->mac_ap, mac_ap, 6);
memcpy(zeiger->mac_sta, mac_sta, 6);
memcpy(zeiger->essid, essid, 32);
zeiger->essidlen = essidlen;

apstaessidcount++;
tmp = realloc(apstaessidliste, (apstaessidcount +1) *APSTAESSIDLIST_SIZE);
if(tmp == NULL)
	{
	printf("failed to allocate memory\n");
	exit(EXIT_FAILURE);
	}
apstaessidliste = tmp;
return;
}
/*===========================================================================*/
uint8_t getessid(uint8_t *tagdata, int taglen, uint8_t *essidstr)
{
int c;
ietag_t *tagl;
tagl = (ietag_t*)tagdata;
while(0 < taglen)
	{
	if(tagl->id == TAG_SSID)
		{
		if((tagl->len == 0) || (tagl->len > 32))
			{
			return 0;
			}
		if(tagl->data[0] == 0)
			{
			return 0;
			}
		/* only allow essids that contains no function characters */
		for(c = 0; c < tagl->len; c++)
			{
			if(tagl->data[c] < 0x20)
				{
				return 0;
				}
			if(tagl->data[c] == 0x7f)
				{
				return 0;
				}
			}
		memcpy(essidstr, tagl->data, tagl->len);
		return tagl->len;
		}
	tagl = (ietag_t*)((uint8_t*)tagl +tagl->len +IETAG_SIZE);
	taglen -= tagl->len;
	}
return 0;
}
/*===========================================================================*/
void process80211beacon(uint32_t ts_sec, int caplen, uint8_t *packet)
{
uint8_t *packet_ptr;
mac_t *macf;
int essidlen;
uint8_t essidstr[32];

if(caplen < (int)MAC_SIZE_NORM +(int)CAPABILITIESAP_SIZE +2)
	{
	return;
	}
macf = (mac_t*)packet;
packet_ptr = packet +MAC_SIZE_NORM +CAPABILITIESAP_SIZE;
memset(&essidstr, 0, 32);
essidlen = getessid(packet_ptr, caplen -MAC_SIZE_NORM -CAPABILITIESAP_SIZE, essidstr);
if(essidlen == 0)
	{
	return;
	}
addapstaessid(ts_sec, macf->addr1, macf->addr2, essidlen, essidstr);
beaconframecount++;
return;
}
/*===========================================================================*/
void process80211probe_req(uint32_t ts_sec, int caplen, uint8_t *packet)
{
uint8_t *packet_ptr;
mac_t *macf;
int essidlen;
uint8_t essidstr[32];

if(caplen < (int)MAC_SIZE_NORM +2)
	{
	return;
	}
macf = (mac_t*)packet;
packet_ptr = packet +MAC_SIZE_NORM;
memset(&essidstr, 0, 32);
essidlen = getessid(packet_ptr, caplen -MAC_SIZE_NORM +2, essidstr);
if(essidlen == 0)
	{
	return;
	}
addapstaessid(ts_sec, macf->addr2, macf->addr1, essidlen, essidstr);
proberequestframecount++;
return;
}
/*===========================================================================*/
void process80211probe_resp(uint32_t ts_sec, int caplen, uint8_t *packet)
{
uint8_t *packet_ptr;
mac_t *macf;
int essidlen;
uint8_t essidstr[32];

if(caplen < (int)MAC_SIZE_NORM +(int)CAPABILITIESAP_SIZE +2)
	{
	return;
	}
macf = (mac_t*)packet;
packet_ptr = packet +MAC_SIZE_NORM +CAPABILITIESAP_SIZE;
memset(&essidstr, 0, 32);
essidlen = getessid(packet_ptr, caplen -MAC_SIZE_NORM -CAPABILITIESAP_SIZE, essidstr);
if(essidlen == 0)
	{
	return;
	}
addapstaessid(ts_sec, macf->addr1, macf->addr2, essidlen, essidstr);
proberesponseframecount++;
return;
}
/*===========================================================================*/
void process80211assoc_req(uint32_t ts_sec, int caplen, uint8_t *packet)
{
uint8_t *packet_ptr;
mac_t *macf;
int essidlen;
uint8_t essidstr[32];

if(caplen < (int)MAC_SIZE_NORM +(int)CAPABILITIESSTA_SIZE +2)
	{
	return;
	}
macf = (mac_t*)packet;
packet_ptr = packet +MAC_SIZE_NORM +CAPABILITIESSTA_SIZE;
memset(&essidstr, 0, 32);
essidlen = getessid(packet_ptr, caplen -MAC_SIZE_NORM -CAPABILITIESSTA_SIZE, essidstr);
if(essidlen == 0)
	{
	return;
	}

addapstaessid(ts_sec, macf->addr2, macf->addr1, essidlen, essidstr);
associationrequestframecount++;
return;
}
/*===========================================================================*/
void process80211assoc_resp()
{

associationresponseframecount++;
return;
}
/*===========================================================================*/
void process80211reassoc_req(uint32_t ts_sec, int caplen, uint8_t *packet)
{
uint8_t *packet_ptr;
mac_t *macf;
int essidlen;
uint8_t essidstr[32];

if(caplen < (int)MAC_SIZE_NORM +(int)CAPABILITIESRESTA_SIZE +2)
	{
	return;
	}
macf = (mac_t*)packet;
packet_ptr = packet +MAC_SIZE_NORM +CAPABILITIESRESTA_SIZE;
memset(&essidstr, 0, 32);
essidlen = getessid(packet_ptr, caplen -MAC_SIZE_NORM -CAPABILITIESRESTA_SIZE, essidstr);
if(essidlen == 0)
	{
	return;
	}
addapstaessid(ts_sec, macf->addr2, macf->addr1, essidlen, essidstr);

reassociationrequestframecount++;
return;
}
/*===========================================================================*/
void process80211reassoc_resp()
{

reassociationresponseframecount++;
return;
}
/*===========================================================================*/
void process80211packet(uint32_t ts_sec, int caplen, uint8_t *packet)
{
mac_t *macf;

if(caplen < MAC_SIZE_NORM)
	{
	return;
	}
macf = (mac_t*)packet;
if(macf->type == IEEE80211_FTYPE_MGMT)
	{
	if(macf->subtype == IEEE80211_STYPE_BEACON)
		{
		process80211beacon(ts_sec, caplen, packet);
		}
	else if (macf->subtype == IEEE80211_STYPE_PROBE_REQ)
		{
		process80211probe_req(ts_sec, caplen, packet);
		}
	else if (macf->subtype == IEEE80211_STYPE_PROBE_RESP)
		{
		process80211probe_resp(ts_sec, caplen, packet);
		}
	else if (macf->subtype == IEEE80211_STYPE_ASSOC_REQ)
		{
		process80211assoc_req(ts_sec, caplen, packet);
		}
	else if (macf->subtype == IEEE80211_STYPE_ASSOC_RESP)
		{
		process80211assoc_resp(ts_sec, caplen, packet);
		}
	else if (macf->subtype == IEEE80211_STYPE_REASSOC_REQ)
		{
		process80211reassoc_req(ts_sec, caplen, packet);
		}
	else if (macf->subtype == IEEE80211_STYPE_REASSOC_RESP)
		{
		process80211reassoc_resp(ts_sec, caplen, packet);
		}
	return;
	}
/*
else if(macf->type == IEEE80211_STYPE_DATA) 
	{
	}
else if(macf->type == IEEE80211_STYPE_QOS_DATA) 
	{
	}

*/


return;
}
/*===========================================================================*/
void processpacket(uint32_t ts_sec, int linktype, int caplen, uint8_t *packet)
{
uint8_t *packet_ptr;
rth_t *rth;
prism_t *prism;
ppi_t *ppi;

packet_ptr = packet;
if(linktype == DLT_IEEE802_11_RADIO)
	{
	if(caplen < (int)RTH_SIZE)
		{
		printf("failed to read radiotap header\n");
		return;
		}
	rth = (rth_t*)packet;
	#ifdef BIG_ENDIAN_HOST
	rth->it_len	 = byte_swap_16(rth->it_len);
	rth->it_present	= byte_swap_32(rth->it_present);
	#endif
	packet_ptr += rth->it_len;
	caplen -= rth->it_len;
	process80211packet(ts_sec, caplen, packet_ptr);
	}
else if(linktype == DLT_IEEE802_11)
	{
	process80211packet(ts_sec, caplen, packet);
	}
else if(linktype == DLT_PRISM_HEADER)
	{
	if(caplen < (int)PRISM_SIZE)
		{
		printf("failed to read prism header\n");
		return;
		}
	prism = (prism_t*)packet;
	#ifdef BIG_ENDIAN_HOST
	prism->msgcode	= byte_swap_32(prism->msgcode);
	prism->msglen	= byte_swap_32(prism->msglen);
	#endif
	packet_ptr += prism->msglen;
	caplen -= prism->msglen;
	process80211packet(ts_sec, caplen, packet_ptr);
	}
else if(linktype == DLT_PPI)
	{
	if(caplen < (int)PPI_SIZE)
		{
		printf("failed to read ppi header\n");
		return;
		}
	ppi = (ppi_t*)packet;
	#ifdef BIG_ENDIAN_HOST
	ppi->pph_len	byte_swap_16(ppi->pph_len);
	#endif
	packet_ptr += ppi->pph_len;
	caplen -= ppi->pph_len;
	process80211packet(ts_sec, caplen, packet_ptr);
	}
return;
}
/*===========================================================================*/
void processpcapng(int fd, char *pcapinname)
{
bool tscleanflag = false;
int endianess = 0;
int pcapreaderrors = 0;
unsigned int res;
unsigned long long int rawpacketcount = 0;
unsigned long long int skippedpacketcount = 0;

block_header_t pcapngbh;
section_header_block_t pcapngshb;
interface_description_block_t pcapngidb;
packet_block_t pcapngpb;
enhanced_packet_block_t pcapngepb;
uint8_t packet[MAXPACPSNAPLEN];

printf("start reading from %s\n", pcapinname);
while(1)
	{
	res = read(fd, &pcapngbh, BH_SIZE);
	if(res == 0)
		{
		break;
		}
	if(res != BH_SIZE)
		{
		pcapreaderrors = 1;
		printf("failed to read pcapng header block\n");
		break;
		}
	if(pcapngbh.block_type == PCAPNGBLOCKTYPE)
		{
		res = read(fd, &pcapngshb, SHB_SIZE);
		if(res != SHB_SIZE)
			{
			pcapreaderrors = 1;
			printf("failed to read pcapng section header block\n");
			break;
			}
		#ifdef BIG_ENDIAN_HOST
		pcapngbh.total_length = byte_swap_32(pcapngbh.total_length);
		pcapngshb.byte_order_magic	= byte_swap_32(pcapngshb.byte_order_magic);
		pcapngshb.major_version		= byte_swap_16(pcapngshb.major_version);
		pcapngshb.minor_version		= byte_swap_16(pcapngshb.minor_version);
		pcapngshb.section_length	= byte_swap_64(pcapngshb.section_length);
		#endif
		if(pcapngshb.byte_order_magic == PCAPNGMAGICNUMBERBE)
			{
			endianess = 1;
			pcapngbh.total_length = byte_swap_32(pcapngbh.total_length);
			pcapngshb.byte_order_magic	= byte_swap_32(pcapngshb.byte_order_magic);
			pcapngshb.major_version		= byte_swap_16(pcapngshb.major_version);
			pcapngshb.minor_version		= byte_swap_16(pcapngshb.minor_version);
			pcapngshb.section_length	= byte_swap_64(pcapngshb.section_length);
			}
		lseek(fd, pcapngbh.total_length -BH_SIZE -SHB_SIZE, SEEK_CUR);
		continue;
		}
	#ifdef BIG_ENDIAN_HOST
	pcapngbh.block_type = byte_swap_32(pcapngbh.block_type);
	pcapngbh.total_length = byte_swap_32(pcapngbh.total_length);
	#endif
	if(endianess == 1)
		{
		pcapngbh.block_type = byte_swap_32(pcapngbh.block_type);
		pcapngbh.total_length = byte_swap_32(pcapngbh.total_length);
		}

	if(pcapngbh.block_type == 1)
		{
		res = read(fd, &pcapngidb, IDB_SIZE);
		if(res != IDB_SIZE)
			{
			pcapreaderrors = 1;
			printf("failed to get pcapng interface description block\n");
			break;
			}
		#ifdef BIG_ENDIAN_HOST
		pcapngidb.linktype	= byte_swap_16(pcapngidb.linktype);
		pcapngidb.snaplen	= byte_swap_32(pcapngidb.snaplen);
		#endif
		if(endianess == 1)
			{
			pcapngidb.linktype	= byte_swap_16(pcapngidb.linktype);
			pcapngidb.snaplen	= byte_swap_32(pcapngidb.snaplen);
			}
		if(pcapngidb.snaplen > MAXPACPSNAPLEN)
			{
			printf("detected oversized snaplen (%d) \n", pcapngidb.snaplen);
			pcapreaderrors = 1;
			}
		lseek(fd, pcapngbh.total_length -BH_SIZE -IDB_SIZE, SEEK_CUR);
		}

	else if(pcapngbh.block_type == 2)
		{
		res = read(fd, &pcapngpb, PB_SIZE);
		if(res != PB_SIZE)
			{
			pcapreaderrors = 1;
			printf("failed to get pcapng packet block (obsolete)\n");
			break;
			}
		#ifdef BIG_ENDIAN_HOST
		pcapngpb.interface_id	= byte_swap_16(pcapngpb.interface_id);
		pcapngpbdrops_count.	= byte_swap_16(pcapngpb.drops_count);
		pcapngpbtimestamp_high.	= byte_swap_32(pcapngpb.timestamp_high);
		pcapngpbtimestamp_low.	= byte_swap_32(pcapngpb.timestamp_low);
		pcapngpb.caplen		= byte_swap_32(pcapngpb.caplen);
		pcapngpb.len		= byte_swap_32(pcapngpb.len);
		#endif
		if(endianess == 1)
			{
			pcapngpb.interface_id	= byte_swap_16(pcapngpb.interface_id);
			pcapngpb.drops_count	= byte_swap_16(pcapngpb.drops_count);
			pcapngpb.timestamp_high	= byte_swap_32(pcapngpb.timestamp_high);
			pcapngpb.timestamp_low	= byte_swap_32(pcapngpb.timestamp_low);
			pcapngpb.caplen		= byte_swap_32(pcapngpb.caplen);
			pcapngpb.len		= byte_swap_32(pcapngpb.len);
			}

		if((pcapngepb.timestamp_high == 0) && (pcapngepb.timestamp_low == 0))
			{
			tscleanflag = true;
			}

		if(pcapngpb.caplen < MAXPACPSNAPLEN)
			{
			res = read(fd, &packet, pcapngpb.caplen);
			if(res != pcapngpb.caplen)
				{
				printf("failed to read packet\n");
				pcapreaderrors = 1;
				break;
				}
			lseek(fd, pcapngbh.total_length -BH_SIZE -PB_SIZE -pcapngepb.caplen, SEEK_CUR);
			rawpacketcount++;
			}
		else
			{
			lseek(fd, pcapngbh.total_length -BH_SIZE -PB_SIZE +pcapngpb.caplen, SEEK_CUR);
			pcapngpb.caplen = 0;
			pcapngpb.len = 0;
			skippedpacketcount++;
			}

		res = read(fd, &packet, pcapngpb.caplen);
		if(res != pcapngpb.caplen)
			{
			printf("failed to read packet\n");
			pcapreaderrors = 1;
			break;
			}

		rawpacketcount++;
		lseek(fd, pcapngbh.total_length -BH_SIZE -PB_SIZE -pcapngpb.caplen, SEEK_CUR);
		}

	else if(pcapngbh.block_type == 3)
		{
		lseek(fd, pcapngbh.total_length -BH_SIZE, SEEK_CUR);
		}

	else if(pcapngbh.block_type == 4)
		{
		lseek(fd, pcapngbh.total_length -BH_SIZE, SEEK_CUR);
		}

	else if(pcapngbh.block_type == 5)
		{
		lseek(fd, pcapngbh.total_length -BH_SIZE, SEEK_CUR);
		}

	else if(pcapngbh.block_type == 6)
		{
		res = read(fd, &pcapngepb, EPB_SIZE);
		if(res != EPB_SIZE)
			{
			pcapreaderrors = 1;
			printf("failed to get pcapng enhanced packet block\n");
			break;
			}
		#ifdef BIG_ENDIAN_HOST
		pcapngepb.interface_id		= byte_swap_32(pcapngepb.interface_id);
		pcapngepb.timestamp_high	= byte_swap_32(pcapngepb.timestamp_high);
		pcapngepb.timestamp_low		= byte_swap_32(pcapngepb.timestamp_low);
		pcapngepb.caplen		= byte_swap_32(pcapngepb.caplen);
		pcapngepb.len			= byte_swap_32(pcapngepb.len);
		#endif
		if(endianess == 1)
			{
			pcapngepb.interface_id		= byte_swap_32(pcapngepb.interface_id);
			pcapngepb.timestamp_high	= byte_swap_32(pcapngepb.timestamp_high);
			pcapngepb.timestamp_low		= byte_swap_32(pcapngepb.timestamp_low);
			pcapngepb.caplen		= byte_swap_32(pcapngepb.caplen);
			pcapngepb.len			= byte_swap_32(pcapngepb.len);
			}

		if((pcapngepb.timestamp_high == 0) && (pcapngepb.timestamp_low == 0))
			{
			tscleanflag = true;
			}

		if(pcapngepb.caplen < MAXPACPSNAPLEN)
			{
			res = read(fd, &packet, pcapngepb.caplen);
			if(res != pcapngepb.caplen)
				{
				printf("failed to read packet\n");
				pcapreaderrors = 1;
				break;
				}
			lseek(fd, pcapngbh.total_length -BH_SIZE -EPB_SIZE -pcapngepb.caplen, SEEK_CUR);
			rawpacketcount++;
			}
		else
			{
			lseek(fd, pcapngbh.total_length -BH_SIZE -EPB_SIZE +pcapngepb.caplen, SEEK_CUR);
			pcapngepb.caplen = 0;
			pcapngepb.len = 0;
			skippedpacketcount++;
			}
		}
	else
		{
		lseek(fd, pcapngbh.total_length -BH_SIZE, SEEK_CUR);
		}
	if(pcapngepb.caplen > 0)
		{
		if(hexmodeflag == true)
			{
			packethexdump(pcapngepb.timestamp_high, pcapngepb.timestamp_low, rawpacketcount, pcapngidb.linktype, pcapngidb.snaplen, pcapngepb.caplen, pcapngepb.len, packet);
			}
		processpacket(pcapngepb.timestamp_high, pcapngidb.linktype, pcapngepb.caplen, packet);
		}
	}

printcapstatus("pcapng", pcapinname, pcapngshb.major_version, pcapngshb.minor_version, pcapngidb.linktype, endianess, rawpacketcount, skippedpacketcount, pcapreaderrors, tscleanflag);
return;
}
/*===========================================================================*/
void processpcap(int fd, char *pcapinname)
{
bool tscleanflag = false;
int endianess = 0;
int pcapreaderrors = 0;
unsigned int res;
unsigned long long int rawpacketcount = 0;
unsigned long long int skippedpacketcount = 0;

pcap_hdr_t pcapfhdr;
pcaprec_hdr_t pcaprhdr;
uint8_t packet[MAXPACPSNAPLEN];

printf("start reading from %s\n", pcapinname);
res = read(fd, &pcapfhdr, PCAPHDR_SIZE);
if(res != PCAPHDR_SIZE)
	{
	printf("failed to read pcap header\n");
	return;
	}

#ifdef BIG_ENDIAN_HOST
pcapfhdr.magic_number	= byte_swap_32(pcapfhdr.magic_number);
pcapfhdr.version_major	= byte_swap_16(pcapfhdr.version_major);
pcapfhdr.version_minor	= byte_swap_16(pcapfhdr.version_minor);
pcapfhdr.thiszone	= byte_swap_32(pcapfhdr.thiszone);
pcapfhdr.sigfigs	= byte_swap_32(pcapfhdr.sigfigs);
pcapfhdr.snaplen	= byte_swap_32(pcapfhdr.snaplen);
pcapfhdr.network	= byte_swap_32(pcapfhdr.network);
#endif

if(pcapfhdr.magic_number == PCAPMAGICNUMBERBE)
	{
	endianess = 1;
	pcapfhdr.version_major	= byte_swap_16(pcapfhdr.version_major);
	pcapfhdr.version_minor	= byte_swap_16(pcapfhdr.version_minor);
	pcapfhdr.thiszone	= byte_swap_32(pcapfhdr.thiszone);
	pcapfhdr.sigfigs	= byte_swap_32(pcapfhdr.sigfigs);
	pcapfhdr.snaplen	= byte_swap_32(pcapfhdr.snaplen);
	pcapfhdr.network	= byte_swap_32(pcapfhdr.network);
	}

while(1)
	{
	res = read(fd, &pcaprhdr, PCAPREC_SIZE);
	if(res == 0)
		{
		break;
		}
	if(res != PCAPREC_SIZE)
		{
		pcapreaderrors = 1;
		printf("failed to read pcap packet header\n");
		break;
		}

	#ifdef BIG_ENDIAN_HOST
	pcaprhdr.ts_sec		= byte_swap_32(pcaprhdr.ts_sec);
	pcaprhdr.ts_usec	= byte_swap_32(pcaprhdr.ts_usec);
	pcaprhdr.incl_len	= byte_swap_32(pcaprhdr.incl_len);
	pcaprhdr.orig_len	= byte_swap_32(pcaprhdr.orig_len);
	#endif
	if(endianess == 1)
		{
		pcaprhdr.ts_sec		= byte_swap_32(pcaprhdr.ts_sec);
		pcaprhdr.ts_usec	= byte_swap_32(pcaprhdr.ts_usec);
		pcaprhdr.incl_len	= byte_swap_32(pcaprhdr.incl_len);
		pcaprhdr.orig_len	= byte_swap_32(pcaprhdr.orig_len);
		}

	if((pcaprhdr.ts_sec == 0) && (pcaprhdr.ts_usec == 0))
		{
		tscleanflag = true;
		}

	if(pcaprhdr.incl_len < MAXPACPSNAPLEN)
		{
		res = read(fd, &packet, pcaprhdr.incl_len);
		if(res != pcaprhdr.incl_len)
			{
			printf("failed to read packet\n");
			pcapreaderrors = 1;
			break;
			}
		rawpacketcount++;
		}
	else
		{
		lseek(fd, pcaprhdr.incl_len, SEEK_CUR);
		pcaprhdr.incl_len = 0;
		pcaprhdr.orig_len = 0;
		skippedpacketcount++;
		}

	if(pcaprhdr.incl_len > 0)
		{
		if(hexmodeflag == true)
			{
			packethexdump(pcaprhdr.ts_sec, pcaprhdr.ts_usec, rawpacketcount, pcapfhdr.network, pcapfhdr.snaplen, pcaprhdr.incl_len, pcaprhdr.orig_len, packet);
			}
		processpacket(pcaprhdr.ts_sec, pcapfhdr.network, pcaprhdr.incl_len, packet);
		}
	}
	
printcapstatus("pcap", pcapinname, pcapfhdr.version_major, pcapfhdr.version_minor, pcapfhdr.network, endianess, rawpacketcount, skippedpacketcount, pcapreaderrors, tscleanflag);
return;
}
/*===========================================================================*/
void processcapfile(char *pcapinname)
{
int pcapr_fd;
uint32_t magicnumber;

beaconframecount = 0;
proberequestframecount = 0;
proberesponseframecount = 0;
associationrequestframecount = 0;
associationresponseframecount = 0;
reassociationrequestframecount = 0;
reassociationresponseframecount = 0;

pcapr_fd = open(pcapinname, O_RDONLY);
if(pcapr_fd == -1)
	{
	return;
	}

magicnumber = getmagicnumber(pcapr_fd);
if(magicnumber == 0)
	{
	printf("failed to get magicnumber from %s\n", basename(pcapinname));
	close(pcapr_fd);
	return;
	}
lseek(pcapr_fd, 0L, SEEK_SET);

apstaessidliste = malloc(APSTAESSIDLIST_SIZE);
if(apstaessidliste == NULL)
	{
	printf("failed to allocate memory\n");
	exit(EXIT_FAILURE);
	}
apstaessidcount = 0;

if((magicnumber == PCAPMAGICNUMBER) || (magicnumber == PCAPMAGICNUMBERBE))
	processpcap(pcapr_fd, pcapinname);

else if(magicnumber == PCAPNGBLOCKTYPE)
	processpcapng(pcapr_fd, pcapinname);
close(pcapr_fd);


if(apstaessidcount > 0) 
	{
	outputlists();
	}

if(apstaessidliste != NULL)
	{
	free(apstaessidliste);
	}
return;
}
/*===========================================================================*/
bool testgzipfile(char *pcapinname)
{
int pcapr_fd;
uint32_t magicnumber;

pcapr_fd = open(pcapinname, O_RDONLY);
if(pcapr_fd == -1)
	{
	return false;
	}
magicnumber = getmagicnumber(pcapr_fd);
close(pcapr_fd);

if((magicnumber &0xffff) != GZIPMAGICNUMBER)
	{
	return false;
	}
printf("unzip from %s not yet\n", pcapinname);
return true;
}
/*===========================================================================*/
__attribute__ ((noreturn))
void version(char *eigenname)
{
printf("%s %s (C) %s ZeroBeat\n", eigenname, VERSION, VERSION_JAHR);
exit(EXIT_SUCCESS);
}
/*---------------------------------------------------------------------------*/
__attribute__ ((noreturn))
void usage(char *eigenname)
{
printf("%s %s (C) %s ZeroBeat\n"
	"usage:\n"
	"%s <options>\n"
	"%s <options> [input.pcap] [input.pcap] ...\n"
	"%s <options> *.cap\n"
	"%s <options> *.*\n"
	"\n"
	"options:\n"
	"-E <file> : output wordlist (autohex enabled) to use as input wordlist for cracker\n"
	"-T <file> : output traffic information list\n"
	"-H <file> : output dump raw packets in hex\n"
	"-h        : show this help\n"
	"-v        : show version\n"
	"\n", eigenname, VERSION, VERSION_JAHR, eigenname, eigenname, eigenname, eigenname);
exit(EXIT_SUCCESS);
}
/*---------------------------------------------------------------------------*/
__attribute__ ((noreturn))
void usageerror(char *eigenname)
{
printf("%s %s (C) %s by ZeroBeat\n"
	"usage: %s -h for help\n", eigenname, VERSION, VERSION_JAHR, eigenname);
exit(EXIT_FAILURE);
}
/*===========================================================================*/
int main(int argc, char *argv[])
{
int auswahl;
int index;
char *eigenpfadname, *eigenname;

eigenpfadname = strdupa(argv[0]);
eigenname = basename(eigenpfadname);

if(globalinit() == false)
	{
	printf("global  ‎initialization failed\n");
	exit(EXIT_FAILURE);
	}
while ((auswahl = getopt(argc, argv, "E:T:H:hv")) != -1)
	{
	switch (auswahl)
		{
		case 'E':
		essidoutname = optarg;
		break;

		case 'T':
		trafficoutname = optarg;
		break;

		case 'H':
		hexmodeflag = true;
		hexmodeoutname = optarg;
		break;

		case 'h':
		usage(eigenname);

		case 'v':
		version(eigenname);

		default:
		usageerror(eigenname);
		}
	}

if(hexmodeflag == true) 
	{
	if((fhhexmode = fopen(hexmodeoutname, "a+")) == NULL)
		{
		fprintf(stderr, "error opening file %s: %s\n", hexmodeoutname, strerror(errno));
		exit(EXIT_FAILURE);
		}
	}

for(index = optind; index < argc; index++)
	{
	if(testgzipfile(argv[index]) == false)
		processcapfile(argv[index]);
	}

if(hexmodeflag == true)
	{
	fclose(fhhexmode);
	}

return EXIT_SUCCESS;
}
/*===========================================================================*/