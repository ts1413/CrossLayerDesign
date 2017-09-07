/*****************************************************/
/*                                                   */
/*  FILE        :Main.c                              */
/*                                                   */
/*****************************************************/
#include "Os.h"
#include "t_syslog.h"
#include "iodefine.h"
#include "ModelCarControl.h"

#include "rlin3x.h"
#include "rcb3.h"

void OperationControl(void);
void rcb3_ErrorCb(uint8 sum, uint8 c, uint8 * g_rcb3_rmsg);

extern sint16 blsm_speed;

/*
 *  PS3のコンローラからのコマンドを受けるタスク
 */
TASK(OperationManagerTask)
{
  unsigned char c;
  static  boolean executed = FALSE;

  /* 初回起動時のみ実行 */
  if (executed == FALSE) {
    syslog(LOG_INFO, "OperationManagerTask : Start!");
    executed = TRUE;
    rlin3x_init();
  }

  while(rlin3x_receive(&c, 1)) {
    if (rcb3_AddReceivedByte(c)) {
      OperationControl();
    }
  }

  TerminateTask();
}


void
OperationControl(void){

  uint16 rcb3_bstate;
  sint8   angle;
  sint8   speed;
  sint8   steern;
  boolean   brake    = FALSE;
  boolean   headlamp = FALSE; //ヘッドランプとフォグランプ
  boolean   foglamp  = FALSE; //使わない
  boolean   winker_l = FALSE;
  boolean   winker_r = FALSE;
  boolean   hazard   = FALSE;

  /* ボタン状態を生成 */
  rcb3_bstate = (g_rcb3_rmsg[RCB3_MSG_OFFSET_BUTTON_H] << 8U)
                            | (g_rcb3_rmsg[RCB3_MSG_OFFSET_BUTTON_L]);

  /* 各ON/OFF切り替え情報 */
  if ((rcb3_bstate & RCB3_MSG_BUTTON_NOUGHT) != 0U) {
    headlamp = TRUE;
  }
  //if ((rcb3_bstate & RCB3_MSG_BUTTON_SQUARE) != 0U) {
  //  foglamp = TRUE;
  //}
  if ((rcb3_bstate & RCB3_MSG_BUTTON_TRIANGLE) != 0U) {
    hazard = TRUE;
  }
  if ((rcb3_bstate & RCB3_MSG_BUTTON_R1) != 0U) {
    winker_r = TRUE;
  }
  if ((rcb3_bstate & RCB3_MSG_BUTTON_L1) != 0U) {
    winker_l = TRUE;
  }
  if ((rcb3_bstate & RCB3_MSG_BUTTON_CROSS) != 0U) {
    brake = TRUE;
  }

  /* 操舵角(-63～64に調整) */
  angle = (-1) * (g_rcb3_rmsg[RCB3_MSG_OFFSET_L_ANALOG_LR] - 64);

  /* 車速(-63～64に調整) */
  speed = (-1) * (g_rcb3_rmsg[RCB3_MSG_OFFSET_R_ANALOG_UD] - 64);

  /* ステアリングニュートラル調整 */
  if ((rcb3_bstate & RCB3_MSG_BUTTON_UP) != 0U) {
    steern = CONTCMD_STEERN_I; /* ステアリングニュートラルを初期値に */
  }
  else if ((rcb3_bstate & RCB3_MSG_BUTTON_RIGHT) != 0U) {
    steern = CONTCMD_STEERN_R; /* ステアリングのニュートラルを右に調整 */
  }
  else if ((rcb3_bstate & RCB3_MSG_BUTTON_LEFT) != 0U) {
    steern = CONTCMD_STEERN_L; /* ステアリングのニュートラルを左に調整 */
  }
  else {
    steern = 0U; /* 要求なし */
  }

  /* g_contcmd の更新 */
  g_contcmd.angle    = angle;
  g_contcmd.speed    = speed;
//  g_contcmd.gain     = gain;
  g_contcmd.steern   = steern;
  g_contcmd.brake    = brake;
  g_contcmd.headlamp = headlamp;
  g_contcmd.foglamp  = foglamp;
  g_contcmd.hazard   = hazard;
  g_contcmd.winker_l = winker_l;
  g_contcmd.winker_r = winker_r;

  /* 制御系へ更新があったことを通知 */
  UpDateContCmd = TRUE;
}

/*
 *  rcb3_AddReceivedByteからのエラーコールバック
 */
void
rcb3_ErrorCb(uint8 sum, uint8 recv_sum, uint8 *p_rcb3_rmsg)
{
  uint32 i;

  syslog(LOG_INFO, "## RCB3 Sum Error: 0x%x != 0x%x", sum, recv_sum);
  for (i = 0; i < RCB3_MSG_LEN; i++) {
    syslog(LOG_INFO, "## rcb3_rmsg[%d]: 0x%x", i, p_rcb3_rmsg[i]);
  }
}