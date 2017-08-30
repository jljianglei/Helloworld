#ifndef _STC_USERIAL_H
#define _STC_USERIAL_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

enum {
     STC_USERIAL_RX_EXIT
};
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
/**
 * @brief send_wakeup_signal end the select_read,when called the if,select_read will return -1 at once 
 *
 * @param sig_cmd STC_USERIAL_RX_EXIT
 *
 * @return 
 */
int send_wakeup_signal(char sig_cmd);
int select_read(int fd,uint8_t* buf,uint16_t len);
uint16_t userial_write(int fd,uint8_t* data,uint16_t len);
#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif/*_STC_USERIAL_H*/
