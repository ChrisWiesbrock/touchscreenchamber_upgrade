
"""
This file is part of the autonomous TouchscreenChamber-HomeCageSystem Project,
and is released under the "CC BY-NC-SA" LICENSE.

You should have received a copy of the license along with this work.
If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

The general structure of this project is based on the TouchScreen Routine published by Wiesbrock et al. 2022.
The routine has been upgraded to run on newer Windows versions, now integrating movie and audiovisual stimulation.
Further, the TouchscreenChamber now uses a microcontroller for improved lick-detection, timing and general
reliability.

Idea: the idea is to run this to make the hallway up and make a udp communicate with the chamber """

from modules.Serial_functions import search_for_microcontroller_by_name, find_teensy_port , receive_bytes_from_teensy


""""TO DO: integrating Object detection and hallway together"""


class Hallway:

    def __init__(self): # let's see what do we need for our inputs
        # just to have them declared
        self.serial_obj = object
        self.teensy_port = object
        self.ID = None

    def call_hallway(self):
        self.initialize_teensy(moduleName="HallwayTeensy4")  # not sure if I need to use this
        self.teensy_port = find_teensy_port()
        self.ID = self.receive_bytes_from_teensy(self.teensy_port)

    def initialize_teensy(self, moduleName="HallwayTeensy4"):
        # For now, I assume that only one chamber is connected per PC. If that changes then we would need to give each
        # Microcontroller a unique identifier and search for this specific setup.
        # run setup without Microcontroller
        test_mode = False  # for debugging
        if not test_mode:
            # If memory serves me well, this just throws an error if the module is not found and the program aborts with
            # this as intended
            self.serial_obj = search_for_microcontroller_by_name(moduleName)
            print('ready')
        else:
            for _ in range(25):
                print("WARNING! The setup is running in test mode. Only for debugging!")
            #
        #
    #

    def receive_bytes_from_teensy(self, teensy_port):
        id_from_teensy = receive_bytes_from_teensy(teensy_port)
        print("Mouse ID", id_from_teensy)
        return id_from_teensy





