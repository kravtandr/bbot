import subprocess
import requests

r = requests.get('http://127.0.0.1:8000')
js = r.json()
f = open("/home/anya/abot/ros/src/abot_sound/abot_speech_to_text/config/abot_dictionary.txt", "w")
f.write(js["commands"])
f = open("/home/anya/abot/ros/src/abot_sound/abot_speech_to_text/config/abot_gram.gram", "w")
f.write(js["grammar"])
subprocess.run('cd /home/anya/ru4sphinx/text2dict ; ./dict2transcript.pl /home/anya/abot/ros/src/abot_sound/abot_speech_to_text/config/abot_dictionary.txt /home/anya/abot/ros/src/abot_sound/abot_speech_to_text/config/abot_dictionary.dic', shell=True)
f = open("/home/anya/abot/ros/src/abot_sound/abot_text_to_speech/scripts/coordinates.csv", "w")
f.write(js["coordinates"])
#result = subprocess.run('. devel/setup.sh ; roslaunch abot_speech_command abot_sound.launch', shell=True)
