#pragma once

#define UI_NUM_AXES      3
#define UI_AXIS_X        0
#define UI_AXIS_Y        1
#define UI_AXIS_Z        2

// definitions common to Grbl_TouchUI

#define UI_VERSION       "prh 1.1.0"
#define UI_VERSION_DATE  "2021-07-15"

#define UI_SCREEN_WIDTH         320
#define UI_SCREEN_HEIGHT        240

#define UI_TOP_MARGIN           35
#define UI_BOTTOM_MARGIN        35
#define UI_MAIN_BUTTON_WIDTH    80
#define UI_INDICATOR_WIDTH      28

#define UI_MAX_TITLE            30          // maximum length of system title
#define UI_MAX_BUTTON            7          // maximum length of a mutable button

#define IND_STATE_NONE         0x00
#define IND_STATE_ENABLED      0x01
#define IND_STATE_READY        0x02
#define IND_STATE_ACTIVE       0x04
#define IND_STATE_ERROR        0x08
#define IND_STATE_ALL          0x0f



// the primary modes of the "main" window
//
//  enum ui_main_mode_t
//  {
//      MAIN_MODE_NONE = 0,
//      MAIN_MODE_IDLE,
//      MAIN_MODE_BUSY,
//      MAIN_MODE_ALARM
//  };
//
//  // once a job has started it must go to
//  // completion, abort, or error.
//
//  enum ui_job_state_t
//  {
//      JOB_STATE_NONE = 0,
//      JOB_STATE_STARTED,
//      JOB_STATE_PAUSED,
//      JOB_STATE_COMPLETE,
//      JOB_STATE_ABORTED,
//      JOB_STATE_ERROR
//  };

// extern const char *uiMainModeName(ui_main_mode_t mode);
// extern const char *uiJobStateName(ui_job_state_t);


//	const char *uiMainModeName(ui_main_mode_t mode)
//	{
//	    switch (mode)
//	    {
//	        case MAIN_MODE_NONE   : return "NONE";
//	        case MAIN_MODE_IDLE   : return "IDLE";
//	        case MAIN_MODE_BUSY   : return "BUSY";
//	        case MAIN_MODE_ALARM  : return "ALARM";
//	    }
//	    return "UNKNOWN_MODE";
//	}
//
//
//	const char *uiJobStateName(ui_job_state_t state)
//	{
//		switch (state)
//		{
//			case JOB_STATE_NONE		: return "NONE";
//			case JOB_STATE_STARTED	: return "STARTED";
//			case JOB_STATE_COMPLETE : return "COMPLETE";
//			case JOB_STATE_ABORTED  : return "ABORTED";
//			case JOB_STATE_ERROR	: return "ERROR";
//		}
//		return "UNKNOWN_JOB";
//	}

//	switch (gc_state.modal.program_flow)
//	{
//		case ProgramFlow::Running:
//			m_job_state = JOB_STATE_STARTED;
//			m_file_pct = sdCard->report_perc_complete();
//			break;
//		case ProgramFlow::Paused:
//			m_job_state = JOB_STATE_PAUSED;
//			m_file_pct = sdCard->report_perc_complete();
//			break;
//		case ProgramFlow::OptionalStop:
//			m_job_state = JOB_STATE_ABORTED;
//			m_file_pct = sdCard->report_perc_complete();
//			break;
//		case ProgramFlow::CompletedM2:
//		case ProgramFlow::CompletedM30:
//			m_job_state = JOB_STATE_COMPLETE;
//			m_file_pct = 100.00;
//			break;
//	}