// The goal is to parse a Multi Program MPEG-2 Transport Stream and create an XML output file
// containing the PID structure of the .ts file.
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <thread> 
#include "tinyxml.h"
using namespace std;

const string PATStr = "0000000000000"; // PAT id in binary string
const string videoStr = "00000010"; // video stream type
const string audioStr = "00000011"; // audio stream type
const int PATId = 0; // PAT id

void makeXML(int PMTId[], int videoId[], int audioId[]);
void getIds(int PMTId[], int videoId[], int audioId[]);
void getPIDPos(ifstream &file, string PID);
string getPID(ifstream &file);
string getByte(ifstream &file);
void getVideoAudio(ifstream &file, string PMT, int &videoId, int &audioId);

int main()
{
  int PMTId[3], videoId[3], audioId[3];

  // thread to get Ids
  thread th1(getIds, PMTId, videoId, audioId);
  th1.join();

  // thread create XML
  thread th2(makeXML, PMTId, videoId, audioId);
  th2.join();

  return 0;
}

// create XML using TinyXML
void makeXML(int PMTId[], int videoId[], int audioId[])
{
  TiXmlDocument doc;
  TiXmlElement* element = new TiXmlElement( "PAT" );
  element->SetAttribute("id", PATId);
  doc.LinkEndChild( element );

  for (int i = 0; i < 3; i++) {
    TiXmlElement* element2 = new TiXmlElement( "Program" );
    element2->SetAttribute("id", PMTId[i]);
    element->LinkEndChild( element2 );

    TiXmlElement* element3 = new TiXmlElement( "Video" );
    element3->SetAttribute("id", videoId[i]);
    element2->LinkEndChild( element3 );

    TiXmlElement* element4 = new TiXmlElement( "Audio" );
    element4->SetAttribute("id", audioId[i]);
    element2->LinkEndChild( element4 );
  }

  doc.SaveFile( "videoIds.xml" );
}

// get PMT, video, and audio ids
void getIds(int PMTId[], int videoId[], int audioId[]) 
{
  string PMTStr[3]; // PMT ids in binary string

  ifstream file ("bin/combine.ts", ios::binary | ios::in);
  if (file.is_open())
  {
    // look for PAT packet
    getPIDPos(file, PATStr);

    // skip to PMT
    file.seekg(12, ios::cur);

    // get PMT ids
    for (int i = 0; i < 3; i++)
    {
      PMTStr[i] = getPID(file);
      PMTId[i] = stoi(PMTStr[i], 0, 2);
      file.seekg(2, ios::cur);
    }

    // get video and audio ids
    for (int i = 0; i < 3; i++)
    {
      getVideoAudio(file, PMTStr[i], videoId[i], audioId[i]);
    }
  } 
}

// seek to position after PID
void getPIDPos(ifstream &file, string PID)
{
  file.seekg(0, ios::beg);

  // skip sync byte
  while(file.seekg(1, ios::cur))
  {
    // end function if PID found
    if (PID == getPID(file)) break;

    // next packet
    file.seekg(185, ios::cur);
  }
}

// return PID from next 2 bytes
string getPID(ifstream &file)
{
  string s;
  char c;

  // get last 5 bits of 1st byte
  file.get(c);
  for (int i = 4; i >= 0; i--)
  {
    int bit = (c >> i) & 1;
    s.append(to_string(bit));
  }

  // get 2nd byte
  file.get(c);
  for (int i = 7; i >= 0; i--)
  {
    int bit = (c >> i) & 1;
    s.append(to_string(bit));
  }

  return s;
}

// return next byte in binary string
string getByte(ifstream &file)
{
  string s;
  char c;

  // get byte
  file.get(c);
  for (int i = 7; i >= 0; i--)
  {
    int bit = (c >> i) & 1;
    s.append(to_string(bit));
  }

  return s;
}

// get video and audio ids
void getVideoAudio(ifstream &file, string PMT, int &videoId, int &audioId)
{
  string type; // stream type in binary

  // look for PMT packet
  getPIDPos(file, PMT);

  // skip to stream type
  file.seekg(14, ios::cur);
  type = getByte(file);

  // check if video or audio type
  if (type == videoStr)
  {
    videoId = stoi(getPID(file), 0, 2);
  }
  else if (type == audioStr)
  {
    audioId = stoi(getPID(file), 0 ,2);
  }

  // skip to next stream type
  file.seekg(2, ios::cur);
  type = getByte(file);

  // check if video or audio type
  if (type == videoStr)
  {
    videoId = stoi(getPID(file), 0, 2);
  }
  else if (type == audioStr)
  {
    audioId = stoi(getPID(file), 0 ,2);
  }
}
