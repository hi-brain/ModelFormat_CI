// -*- C++ -*-
/*!
 * @file  ModuleName.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "ModuleName.h"
using namespace std;
// Module specification
// <rtc-template block="module_spec">
static const char* modulename_spec[] =
  {
    "implementation_id", "ModuleName",
    "type_name",         "ModuleName",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "VenderName",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.ReadData", "0:camera",
    "conf.default.viewer", "0:OFF",
    "conf.default.device_num", "0",
    "conf.default.filename", "image/image???.jpg",
    "conf.default.log_sampling", "0:OFF",
    "conf.default.parvo_view", "0:OFF",
    "conf.default.magno_view", "0:OFF",
    // Widget
    "conf.__widget__.ReadData", "radio",
    "conf.__widget__.viewer", "radio",
    "conf.__widget__.device_num", "text",
    "conf.__widget__.filename", "text",
    "conf.__widget__.log_sampling", "radio",
    "conf.__widget__.parvo_view", "radio",
    "conf.__widget__.magno_view", "radio",
    // Constraints
    "conf.__constraints__.ReadData", "(1:picture,0:camera)",
    "conf.__constraints__.viewer", "(1:ON,0:OFF)",
    "conf.__constraints__.log_sampling", "(1:ON,0:OFF)",
    "conf.__constraints__.parvo_view", "(1:ON,0:OFF)",
    "conf.__constraints__.magno_view", "(1:ON,0:OFF)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ModuleName::ModuleName(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_inputIn("input", m_input),
    m_outputOut("output", m_output)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ModuleName::~ModuleName()
{
}



RTC::ReturnCode_t ModuleName::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("input", m_inputIn);

  // Set OutPort buffer
  addOutPort("output", m_outputOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("ReadData", m_ReadData, "0:camera");
  bindParameter("viewer", imageviewer.view, "0:OFF");
  bindParameter("device_num", m_device_num, "0");
  bindParameter("filename", m_filename, "image/image???.jpg");
  bindParameter("log_sampling", m_log_sampling, "0:OFF");
  //bindParameter("parvo_view", parvoviewer.view, "0:OFF");
  //bindParameter("magno_view", magnoviewer.view, "0:OFF");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ModuleName::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ModuleName::onActivated(RTC::UniqueId ec_id)
{
  //ReadImage
  if(!m_ReadData){
    videoCapture.open(m_device_num);
    videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    if (!videoCapture.isOpened())
    {
      cout<<"No Camera Device"<<endl;
      return RTC::RTC_ERROR;
    }
  }
  //Face_Detector
/*
  cascadeName = "/usr/local/opencv3.1.0/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml";
  if(!cascade.load(cascadeName))
    return RTC::RTC_ERROR;
  m_output.width = 0;
	m_output.height = 0;
*/
  imageviewer.initialize(m_outputOut.getName(), &outputFrame);
  //parvoviewer.initialize("parvo", &parvoFrame);
  //magnoviewer.initialize("magno", &magnoFrame);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ModuleName::onDeactivated(RTC::UniqueId ec_id)
{
  inputFrame.release();
	outputFrame.release();
  imageviewer.vswitch(0);
  count = 0;
	//バッファの初期化
	while(m_inputIn.isNew()) m_inputIn.read();
/*
  retina_image->clearBuffers();
  parvoFrame.release();
	magnoFrame.release();
  parvoviewer.vswitch(0);
  magnoviewer.vswitch(0);
*/
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ModuleName::onExecute(RTC::UniqueId ec_id)
{
  //ReadImage
  if(!m_ReadData){
    if (!videoCapture.isOpened())
       return RTC::RTC_OK;
    videoCapture>>inputFrame;
  }else{
    inputFrame = cv::imread(m_filename, 1);
    if(inputFrame.empty()){cout << " filename not found" << endl;return RTC::RTC_ERROR;}
  }

  /*
    if (!m_inputIn.isNew()) // Check input image is new
    {
  	return RTC::RTC_OK;
    }
    m_inputIn.read();
    m_input>>inputFrame;
  */

  outputFrame = inputFrame;	                          //shallow copy
  //cvtColor( inputFrame, outputFrame, CV_BGR2GRAY);  //Gray scale
  //Retina
  /*
    if(fflg == false)	//一回目の初期化処理
    {
      if(m_log_sampling == 0){
        retina_image = cv::bioinspired::createRetina(inputFrame.size());
      }
      else{
        retina_image = cv::bioinspired::createRetina(inputFrame.size(), true, cv::bioinspired::RETINA_COLOR_BAYER, true, 2.0, 10.0);
      }
      fflg = true;
    }
    retina_image->run(inputFrame);
    retina_image->getParvo(parvoFrame);
    retina_image->getMagno(magnoFrame);
    parvoviewer.vswitch();
    magnoviewer.vswitch();
  */
//Face_Detector
/*
	scale = 1.0;
  cv::Mat smallImg(cv::saturate_cast<int>(inputFrame.rows/scale), cv::saturate_cast<int>(inputFrame.cols/scale), CV_8UC1);
  cv::cvtColor(inputFrame, gray, CV_BGR2GRAY);
  cv::resize(gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
  cascade.detectMultiScale(smallImg, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
  std::vector<cv::Rect>::const_iterator r = faces.begin();
  for(; r != faces.end(); ++r)
  {
    cv::Point center;
    int radius;
    center.x = cv::saturate_cast<int>((r->x + r->width*0.5)*scale);
    center.y = cv::saturate_cast<int>((r->y + r->height*0.5)*scale);
    radius = cv::saturate_cast<int>((r->width + r->height)*0.25*scale);
    cv::circle( outputFrame, center, radius, cv::Scalar(80,80,255), 3, 8, 0 );
  }
*/
  outputFrame>>m_output;
  imageviewer.vswitch();
	m_outputOut.write();

  if (count > 100)
  {
    count = 0;
    coil::TimeValue tm;
    tm = coil::gettimeofday();

    double sec(tm - tm_pre);

    if (sec > 1.0 && sec < 1000.0)
    {
        std::cout << 100/sec << " [FPS]" << std::endl;
    }

    tm_pre = tm;
  }
  ++count;
  return RTC::RTC_OK;

}

/*
RTC::ReturnCode_t ModuleName::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

extern "C"
{

  void ModuleNameInit(RTC::Manager* manager)
  {
    coil::Properties profile(modulename_spec);
    manager->registerFactory(profile,
                             RTC::Create<ModuleName>,
                             RTC::Delete<ModuleName>);
  }

};
