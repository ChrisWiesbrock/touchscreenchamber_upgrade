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

from os import path, makedirs, getcwd
from psychopy import visual, core, data, event, logging, sound, gui
from Hallway import Hallway


if __name__ == "__main__":
    hallway_code=Hallway()
    hallway_code.call_hallway()

    print("hallway is runing!")