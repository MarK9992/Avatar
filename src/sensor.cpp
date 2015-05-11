#include "sensor.h"
#include <iostream>

using namespace openni;
using namespace std;

CSensor::CSensor()
{

}

CSensor::~CSensor()
{
    cout << "Device shutting down..." << endl;
    // arret et destruction des flux videos
    m_colorStream.stop();
    m_colorStream.destroy();
    m_depthStream.stop();
    m_depthStream.destroy();

    // fermeture de la session OpenNI
    OpenNI::shutdown();
    cout << "DONE" << endl;
}

bool CSensor::OnInit(bool show_color_stream)
{
    // initialisation d'OpenNI
    if(OpenNI::initialize() != STATUS_OK) {cout << OpenNI::getExtendedError() << endl; return false;}

    // ouverture du peripherique de detection
    if(m_device.open(ANY_DEVICE) != STATUS_OK) {cout << OpenNI::getExtendedError() << endl; return false;}

    // creation du flux video couleur
    if(m_colorStream.create(m_device, SENSOR_COLOR) != STATUS_OK) {cout << OpenNI::getExtendedError() << endl; return false;}
    if(m_colorStream.start() != STATUS_OK) {cout << OpenNI::getExtendedError() << endl; return false;}
    if(!m_colorStream.isValid()) return false;

    // creation du flux video profondeur
    if(m_depthStream.create(m_device, SENSOR_DEPTH) != STATUS_OK) {cout << OpenNI::getExtendedError() << endl; return false;}
    if(m_depthStream.start() != STATUS_OK) {cout << OpenNI::getExtendedError() << endl; return false;}
    if(!m_depthStream.isValid()) return false;

    // affectation du flux video actif
    if(show_color_stream) active_stream = color_stream;
    else active_stream = depth_stream;

    // proprietes de detection
    if(m_device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR)) {
        if(m_device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR) != STATUS_OK) {
            cout << OpenNI::getExtendedError() << endl; return false;
        }
    }
    if(m_device.setDepthColorSyncEnabled(!show_color_stream) != STATUS_OK) {
        cout << OpenNI::getExtendedError() << endl; return false;
    }

    return true;
}

void CSensor::SwitchActiveStream(EActiveStream stream) {
    switch (stream) {
    case color_stream:
        active_stream = color_stream;
        if(m_device.setDepthColorSyncEnabled(false) != STATUS_OK)
            cout << OpenNI::getExtendedError() << endl;
        break;
    case depth_stream:
        active_stream = depth_stream;
        if(m_device.setDepthColorSyncEnabled(true) != STATUS_OK)
            cout << OpenNI::getExtendedError() << endl;
        break;
    default:;
    }
}
