/*
 * mainwindow.cpp
 * The main window for the QT Application
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.eeng.dcu.ie/~molloyd/
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL I
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SimpleGPIO.h"
#include "BMA180Accelerometer.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Green LED Pin
    // use SimpleGPIO.h to setup the LED Pin correctly as an output pin using MODE 7 (GPIO0_7)
    // if you don't understand these steps watch my video on YouTube
    //    "Beaglebone: GPIO Programming on ARM Embedded Linux"

    gpio_omap_mux_setup("eCAP0_in_PWM0", "07"); 	//gpio0_7 P9 header pin# 42
    this->GREEN_LED_PIN = 7;  //gpio number (0x32)+7 = 7  state of the class for sharing
    gpio_export(this->GREEN_LED_PIN);
    gpio_set_dir(this->GREEN_LED_PIN, OUTPUT_PIN);

    // Set up the Accelerometer

    this->accelerometer = new BMA180Accelerometer(3, 0x40);
    this->accelerometer->setBandwidth(BW_150HZ);
    this->accelerometer->setRange(PLUSMINUS_1_G);

    autoLED(false);

    connect(&this->dataTimer, SIGNAL(timeout()), this, SLOT(newAccelerometerDataSlot()));
    this->dataTimer.start(100);
}

// My manually created Slot
void MainWindow::newAccelerometerDataSlot()
{
    accelerometer->readFullSensorState();
    double p = accelerometer->getPitch();
    double r = accelerometer->getRoll();
    ui->pitchSlider->setValue((int)p);
    ui->rollSlider->setValue((int)-r);

    QString pitchString = "", rollString = "";
    pitchString.append(QString("%1").arg((int)p));
    rollString.append(QString("%1").arg((int)-r));
    ui->pitchNumber->display(pitchString);
    ui->rollNumber->display(rollString);

    int dialValue = ui->lightLevelDial->value();
    if(this->autoLEDState){
        if(((int)p)>=dialValue || ((int)-r)>=dialValue){
            ::gpio_set_value(this->GREEN_LED_PIN, HIGH);
        }
        else{
            ::gpio_set_value(this->GREEN_LED_PIN, LOW);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ledOnButton_clicked()
{
    ::gpio_set_value(this->GREEN_LED_PIN, HIGH);
    autoLED(false);
}

void MainWindow::on_ledOffButton_clicked()
{
    ::gpio_set_value(this->GREEN_LED_PIN, LOW);
    autoLED(false);
}

void MainWindow::on_lightLevelDial_valueChanged(int value)
{
    QString temp = "";
    temp.append(QString("%1").arg(value));
    ui->lightLevelEdit->clear();
    ui->lightLevelEdit->insert(temp);
}

void MainWindow::on_ledAutoButton_clicked()
{
    autoLED(true);
}

void MainWindow::autoLED(bool state){
    if(state){
        this->autoLEDState = true;
        ui->statusBar->showMessage("Auto LED Enabled");
    }
    else{
        this->autoLEDState = false;
        ui->statusBar->showMessage("Auto LED Disabled");
    }
}
