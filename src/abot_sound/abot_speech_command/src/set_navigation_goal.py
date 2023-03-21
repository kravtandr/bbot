#!/usr/bin/env python3
# coding: utf-8

import rospy
import pandas as pd
from std_msgs.msg import String

coords = pd.read_csv("/home/anya/abot/ros/src/abot_sound/abot_speech_command/src/coordinates.csv")

class SetNavigationGoal(object):
	def __init__(self):
		rospy.init_node('set_navigation_goal')
		rospy.on_shutdown(self.shutdown)
		rospy.Subscriber('/abot/stt/grammar_data', String, self.grammarCallback)
		self._tts_pub = rospy.Publisher('/abot/tts/text_to_say', String, queue_size=10)
		rospy.spin()

	def grammarCallback(self, text_msg):
		rospy.loginfo("Voice command node 'Set Navigation Goal': Start. The text message: %s", text_msg.data)
		row = coords[coords['id'] == text_msg.data]
		if row.empty:
			msg = "нет такой аудитории"
		else:
			msg = row.iloc[0]['name']
			rospy.loginfo("Coordinates: %s %s", row.iloc[0]['x'], row.iloc[0]['y'])
			rospy.loginfo("The destination: %s", msg)
		self._tts_pub.publish(msg)

	@staticmethod
	def shutdown():
		rospy.loginfo("Navigation control node: Stop SetNavigationGoal")
		rospy.sleep(1)

if __name__ == "__main__":
	SetNavigationGoal()
