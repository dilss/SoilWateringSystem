# Soil Watering System
Project of a long range wireless communication system and devices, tested especifically on crops
initiated by Edilson Fernandes on november 11, 2017.

This is a very first version of a project that is intended to help on soil maintenance watering level for crops.
The main goal of this project is to work as a long range communication system  that allows farmers to monitor their crops
that is far from their homes. For that, the client part of the system (the remote device in the field) is intended to be of
a low power consumption given once that we want to power with a solar panel(15cmx15cm). The panel will be in charge of power 
the system and charge the battery during day ligth. During nigth the system will be powered by a small battery.

We are using the HC12 Transmitter/Receiver Radio module to perform communication between the client device(on the field) and
the server device(at home atached to a computer and, unlike the client,  without power restrictions). This module allows com-
munication with distances up to 1.8 Km and, luck for us, is very cheap(about 4 bucks). 

For moisture monitoring we will use the SparkFun Soil Moisture Sensor, a sensor that use the resistence between two probes
to tell if a soil is dry or not. 

For hookup this system you will need:

* At least two arduino boards, or the standalone arduino of you design(we pretend to do improments like this in the future);
* One HC12 module atached on each arduino board;
* One SparkFun Soil Moisture Sensor for every arduino you will put on the field;
* A 5 volts battery with tha battery pack an cable;
(We will make the solar part after we test the code and fixed the bugs)

obs: There are many possibilities for this system, including the most obvious one, that is  to buy a solenoid valve


The workings of the code are placed as comments on all its extention, but for summarize:

* We use the avr libraries, sleep.h and power.h to perform our low consumption application;
* On the client side, for every device it is given an ID, that is used to stabilish communication with the server;
* A request of communication is send to the server and if he is doing notthing he responds with an OK;
* The client does not send the data until the OK is received from server; 
* We fill the code with a bunch of serial printing to observe the wrokings, this can be removed for real embedded applications
* The pins are configured as you desired, as long as the HC12 pins are digital and the sensor pins are analog
* Breadboard an male to male jumpers.

The following statement and terms of use are included from the original source code from Joop Brokking's YMFC-AL Project

/////////////////////////////////////////////////////////////////////////////////////// //Terms of use /////////////////////////////////////////////////////////////////////////////////////// //THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, //FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE //AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN //THE SOFTWARE. /////////////////////////////////////////////////////////////////////////////////////// //Safety note /////////////////////////////////////////////////////////////////////////////////////// //Always remove the propellers and stay away from the motors unless you //are 100% certain of what you are doing. ///////////////////////////////////////////////////////////////////////////////////////

