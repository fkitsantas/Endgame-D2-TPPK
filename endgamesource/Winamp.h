extern bool MoveAmp;
extern bool OnAmp;

#pragma once

char* GetCurrentWinampTrack(void);
//Winamp.cpp -- Winamp controls!
char* AmpControl_GetPlayingTitle(void);
void SendW3Key(HWND hwnd_winamp,char message);
HWND FindPlayer(int wanted);
void NextSong();
void PrevSong();
void StopSong();
void PlayPause();
void FastForward();
void Rewind();
void RaiseVolume();
void LowerVolume();
void Shuffle();
void Repeat();
void Close();
//char currTitle[2048];

#if !defined(AMPCONTROL_H)
#define AMPCONTROL_H
// Winamp 2 defines
#define W2_PREV             40044
#define W2_PLAY             40045
#define W2_PAUSE            40046
#define W2_STOP             40047
#define W2_NEXT             40048
#define W2_FILEPLAY         40029 

#define W2_ISPLAYING        104
#define W2_OUTPUTTIME       105
#define W2_JUMPTOTIME       106
#define W2_SETPLAYLISTPOS   121
#define W2_GETLISTLENGTH    124
#define W2_GETLISTPOS       125
#define W2_GETINFO          126


// Winamp 3 defines
#define W3_PREV	            'z'
#define W3_PLAY		        'x'
#define W3_PAUSE		    'c'
#define W3_STOP		        'v'
#define W3_NEXT	            'b'
#define W3_FILEPLAY	        'l'

// Sonique 1 defines
#define S1_PREV	            'z'
#define S1_PLAY		        'x'
#define S1_PAUSE		    'c'
#define S1_STOP		        'v'
#define S1_NEXT             'b'
#define S1_FILEPLAY         'l'

#endif



