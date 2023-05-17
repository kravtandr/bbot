#!/usr/bin/env python
# coding: utf-8

import rospy
import pandas as pd
import time
from std_msgs.msg import String, Bool
from sound_play.libsoundplay import SoundClient

coords = pd.read_csv("/home/anya/abot/ros/src/abot_sound/abot_text_to_speech/scripts/coordinates.csv")
class FestvoxTTS(object):
	def __init__(self):
		rospy.init_node('festvox_tts')
		rospy.on_shutdown(self.shutdown)
		self._volume = rospy.get_param('~volume', 1.0)
		self._voice = rospy.get_param('~voice', 'voice_msu_ru_nsh_clunits')
		self._soundhandle = SoundClient(blocking=True)
		rospy.sleep(1)
		rospy.Subscriber('/abot/stt/grammar_data', String, self.processText)
		self._pub = rospy.Publisher('/abot/tts/speaking_in_progress', Bool, queue_size=1)
		self._coord = rospy.Publisher('/abot/tts/coordinates', String, queue_size=10)
		rospy.loginfo("Festival TTS node: Start")
		rospy.spin()

	def processText(self, text_msg):
		rospy.loginfo("Festival TTS node: Got a string: %s", text_msg.data)
		row = coords[coords['name'] == text_msg.data]
		cancel = ["отмена", "стоп", "стой", "погоди"]
		if text_msg.data in cancel:
			msg = "отмена операции"
			coord = "отмена операции"
		elif row.empty:
			msg = "нет такой аудитории"
			coord = "нет такой аудитории"
		else:
			msg = text_msg.data
			msg_id = row.iloc[0]['id']
			rospy.logwarn('Festival TTS node: OUTPUT - \"' + msg_id + '\"')
			coord = "Coordinates: {0} {1}".format(row.iloc[0]['x'], row.iloc[0]['y'])
		self._coord.publish(coord)
		self._pub.publish(True)
		self._soundhandle.say(msg, self._voice, self._volume)
		self._pub.publish(False)

	@staticmethod
	def shutdown():
		rospy.loginfo("Festival TTS node: Stop")
		rospy.sleep(1)

if __name__ == "__main__":
	FestvoxTTS()
