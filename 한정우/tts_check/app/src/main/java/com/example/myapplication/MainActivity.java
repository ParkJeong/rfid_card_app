package com.example.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;

public class MainActivity extends AppCompatActivity implements OnInitListener{

    private TextToSpeech myTTS;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        myTTS =new TextToSpeech(this,this);
    }

    public void onInit(int status)
    {
        String myText1="안녕하세요 컴퓨터공학과 한정우입니다.";
        String myText2="말하는 거 테스트 중입니다.";
        myTTS.speak(myText1,TextToSpeech.QUEUE_FLUSH,null);
        myTTS.speak(myText2,TextToSpeech.QUEUE_ADD,null);
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        myTTS.shutdown();

    }

}
