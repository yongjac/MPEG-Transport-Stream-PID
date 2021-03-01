Programing task:
Writing a multithreaded C++ app that will runs on a Ubuntu 18.04 Virtual machine.
The goal is to parse a Multi Program MPEG-2 Transport Stream and create an XML output file
containing the PID structure of the .ts file.
Replace id with the PID number:
- PAT id is the PAT pid number.
- Program Id is the PMT Pid number.
- Video and Audio ids are the video and audio pids.

3 videos segments were downloaded from https://www.w6rz.net/ from the Big Buck Bunny (Open Source Movie)

mmpeg was installed using the command:
sudo apt install ffmpeg

The 3 videos were combined using the following command:
ffmpeg -i bbb24p_00 -i bbb24p_01 -i bbb24p_02 -i bbb24p_03 \
-map 0:0 -map 0:1 -map 1:0 -map 1:1 -map 2:0 -map 2:1 \
-program program_num=1:title=video1:st=0:st=1 -program program_num=2:title=video2:st=2:st=3 \ 
-program program_num=3:title=video3:st=4:st=5 \
-f mpegts combine.ts 

To build program use the command:
make

To run program use the command:
./task
