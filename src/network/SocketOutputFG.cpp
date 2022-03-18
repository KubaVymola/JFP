#include "SocketOutputFG.h"

#include <iostream>
#include <stdio.h>
#include <math.h>

SocketOutputFG::SocketOutputFG(const int port) {
    this->socket = new SocketOutput(port);
}

SocketOutputFG::~SocketOutputFG() {
    delete this->socket;
}

void SocketOutputFG::SendHeaders() const {
    if (!socket->CanSend()) return;
    
    socket->Clear();
    socket->Clear("<LABELS>");

    socket->Append("Time");
    
    socket->Append("alt-asl-fg");
    socket->Append("lat-deg");
    socket->Append("long-deg");
    
    socket->Append("phi-deg");
    socket->Append("tht-deg");
    socket->Append("psi-deg");
    
    socket->Append("alpha-deg");
    socket->Append("beta-deg");
    

    socket->Send();
}

void SocketOutputFG::SendData(FDMData& fdmData) const {
    if (!socket->CanSend()) return;
    
    socket->Clear();

    // socket->Append(fdmData.time);

    // socket->Append(fdmData.altitude_asl_ft);
    // socket->Append(fdmData.latitude_deg);
    // socket->Append(fdmData.longitude_deg);

    // socket->Append(fdmData.phi_deg);
    // socket->Append(fdmData.theta_deg);
    // socket->Append(fdmData.psi_deg);
    
    // socket->Append(fdmData.alpha_deg);
    // socket->Append(fdmData.beta_deg);

    // std::cout << fdmData.time << std::endl;

    socket->Send();
}
