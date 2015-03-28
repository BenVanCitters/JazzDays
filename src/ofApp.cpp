#include "ofApp.h"

// comparison routine for sort...
bool sortVertically(  basicSprite * a, basicSprite * b ) {
    return a->pos.y > b->pos.y;
}

//--------------------------------------------------------------
void testApp::setup()
{
    cout << "AudioShader" << endl;
    audioShader.load("audioShader.vert", "audioShader.frag", "");
    audioXAttribLocation = audioShader.getAttributeLocation("texCoordX");
    audioYAttribLocation = audioShader.getAttributeLocation("texCoordY");
    
    camWidth 		= 640;	// try to grab at this size.
    camHeight 		= 480;
    hCount = 100;
    vCount = 70;
    
    ofHideCursor();
    ofEnableNormalizedTexCoords();
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    ofSetFrameRate(30); //lets run at 30 fps!
    
    ofEnableAlphaBlending(); // turn on alpha blending. important!
    charToIndexMap.clear();
    charToIndexMap[' ']=0; charToIndexMap['!']=1; charToIndexMap['\"']=2; charToIndexMap['#']=3;
    charToIndexMap['$']=4; charToIndexMap['%']=5; charToIndexMap['&']=6; charToIndexMap['\'']=7;
    charToIndexMap['(']=8; charToIndexMap[')']=9; charToIndexMap['*']=10; charToIndexMap['+']=11;
    charToIndexMap[',']=12; charToIndexMap['-']=13; charToIndexMap['.']=14; charToIndexMap['/']=15;
    charToIndexMap['0']=16; charToIndexMap['1']=17; charToIndexMap['2']=18; charToIndexMap['3']=19;
    charToIndexMap['4']=20; charToIndexMap['5']=21; charToIndexMap['6']=22; charToIndexMap['7']=23;
    charToIndexMap['8']=24; charToIndexMap['9']=25; charToIndexMap[':']=26; charToIndexMap[';']=27;
    charToIndexMap['<']=28; charToIndexMap['=']=29; charToIndexMap['>']=30; charToIndexMap['?']=31;
    charToIndexMap['@']=32; charToIndexMap['A']=33; charToIndexMap['B']=34; charToIndexMap['C']=35;
    charToIndexMap['D']=36; charToIndexMap['E']=37; charToIndexMap['F']=38; charToIndexMap['G']=39;
    charToIndexMap['H']=40; charToIndexMap['I']=41; charToIndexMap['J']=42; charToIndexMap['K']=43;
    charToIndexMap['L']=44; charToIndexMap['M']=45; charToIndexMap['N']=46; charToIndexMap['O']=47;
    charToIndexMap['P']=48; charToIndexMap['Q']=49; charToIndexMap['R']=50; charToIndexMap['S']=51;
    charToIndexMap['T']=52; charToIndexMap['U']=53; charToIndexMap['V']=54; charToIndexMap['W']=55;
    charToIndexMap['X']=56; charToIndexMap['Y']=57; charToIndexMap['Z']=58; charToIndexMap['[']=59;
    charToIndexMap['\\']=60; charToIndexMap[']']=61; charToIndexMap['^']=62; charToIndexMap['_']=63;
    charToIndexMap['\'']=64; charToIndexMap['a']=65; charToIndexMap['b']=66; charToIndexMap['c']=67;
    charToIndexMap['d']=68; charToIndexMap['e']=69; charToIndexMap['f']=70; charToIndexMap['g']=71;
    charToIndexMap['h']=72; charToIndexMap['i']=73; charToIndexMap['j']=74; charToIndexMap['k']=75;
    charToIndexMap['l']=76; charToIndexMap['m']=77; charToIndexMap['n']=78; charToIndexMap['o']=79;
    charToIndexMap['p']=80; charToIndexMap['q']=81; charToIndexMap['r']=82; charToIndexMap['s']=83;
    charToIndexMap['t']=84; charToIndexMap['u']=85; charToIndexMap['v']=86; charToIndexMap['w']=87;
    charToIndexMap['x']=88; charToIndexMap['y']=89; charToIndexMap['z']=90; charToIndexMap['{']=91;
    charToIndexMap['|']=92; charToIndexMap['}']=93; charToIndexMap['~']=94;
    
    initSprites();
    letterOrder = " .-_':,;^~=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q";
    setupFlagVerts();
}

void testApp::initSprites()
{
    sprites.clear();
    //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 'sidLen'
    float sideLen = 256/10.f;
    spriteRenderer = new ofxSpriteSheetRenderer(1, hCount*vCount, 0, sideLen);
    
    // load the spriteSheetExample.png texture of size 256x256 into the sprite sheet. set it's scale mode to nearest since it's pixel art
    spriteRenderer->loadTexture("m.png", 256, GL_LINEAR);
    
    string s = " !\"#$%&'()*+_@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~";
    float scalingX = ofGetWindowWidth()*1.f/hCount;
    float scalingY = ofGetWindowHeight()*1.f/vCount;
    for(int i=0;i<hCount;i++)
    {
        for(int j=0;j<vCount;j++)
        {
            basicSprite * newSprite = new basicSprite(); // create a new sprite

            newSprite->pos.set(i*scalingX,j*scalingY); //set its position
            
            //set its animation to the walk animation we declared
            newSprite->animation = walkAnimation;
            
            //adjust its frame duration based on how fast it is walking (faster = smaller)
            newSprite->animation.frame_duration /= newSprite->speed;
            
            //change the start index of our sprite. we have 4 rows of animations and our spritesheet is 8 tiles wide, so our possible start indicies are 0, 8, 16, and 24
            newSprite->animation.index = 0;//(int)ofRandom(0,10)*ofRandom(0,10);
            sprites.push_back(newSprite); //add our sprite to the vector
        }
    }

}

//--------------------------------------------------------------
void testApp::update()
{
    vidGrabber.update();
    ofPixelsRef pixelsRef = vidGrabber.getPixelsRef();
    //get min/max of brightness
    float maxBright = ofColor().limit(); //normalizer for brighness values
    float minVBright = 999;
    float maxVBright = -999;
    for (int i = 0; i < camWidth; i+= 7){
        for (int j = 0; j < camHeight; j+= 9){
            // get the pixel and its lightness (lightness is the average of its RGB values)
            float lightness = pixelsRef.getColor(i,j).getLightness()/maxBright;
            maxVBright = MAX(maxVBright,lightness);
            minVBright = MIN(minVBright, lightness);
        }
    }
    
    spriteRenderer->clear(); // clear the sheet
    spriteRenderer->update(ofGetElapsedTimeMillis()); //update the time in the renderer, this is necessary for animations to advance
    
    sort( sprites.begin(), sprites.end(), sortVertically ); // sorts the sprites vertically so the ones that are lower are drawn later and there for in front of the ones that are higher up
    //cache some vaules we use in the loop
    int windW = ofGetWindowWidth();
    int windH = ofGetWindowHeight();
   
    float brightDiff = maxVBright-minVBright;
    if(sprites.size()>0) // if we have sprites
    {
        for(int i=sprites.size()-1;i>=0;i--) //go through them
        {
            //get the brightness of the video input that sits behind this letter
            ofVec2f movieCoord =ofVec2f(camWidth * sprites[i]->pos.x/windW,
                                        camHeight * sprites[i]->pos.y/windH);
            float lightness = pixelsRef.getColor(movieCoord.x, movieCoord.y).getLightness()/maxBright;
            lightness -= minVBright;
            lightness *= brightDiff;
            int charIndex = (int)(lightness*(letterOrder.length()-1));
            
            //set the sprite to match the brightness
            sprites[i]->animation.index = charToIndexMap[letterOrder[charIndex]];
            
            // add them to the sprite renderer (add their animation at their position, there are a lot more options for what could
            // happen here, scale, tint, rotation, etc, but the animation, x and y are the only variables that are required)
            // int layer = -1,                        flipDirection f = F_NONE, float scale = 1.0,
            spriteRenderer->addCenteredTile(&sprites[i]->animation,
                                            sprites[i]->pos.x,
                                            sprites[i]->pos.y,-1,F_NONE,.5);
        }
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(0, 0, 0,255);
    spriteRenderer->draw(); //draw the sprites!
    
//    spriteRenderer->frameBuffer.draw(0,0);
//    spriteRenderer->texture->draw(0, 0);
    
    int count = spriteRenderer->frameBuffer.getNumTextures();
    if(count > 0)
    {
        audioShader.begin();
        
//        ofTexture texRef = spriteRenderer->t;//  //fingerMovie.getTextureReference();
//        texRef.setTextureWrap(GL_REPEAT, GL_REPEAT);
        vidGrabber.update();
        ofTexture texRef = vidGrabber.getTextureReference();
        texRef.setTextureWrap(GL_REPEAT, GL_REPEAT);

        audioShader.setUniformTexture("mytex",spriteRenderer->frameBuffer.getTextureReference(), 0);
//        glBindTexture(        audioShader.getAttributeLocation("myTex"), spriteRenderer->frameBuffer.getTextureReference());
        audioShader.setUniform1f("time", ofGetElapsedTimef());
        audioStrip.draw(GL_TRIANGLE_STRIP, 0, 6*100*100);
//        audioStrip.drawElements(GL_TRIANGLE_STRIP, 6*100*100);
        
        //ofRect(0,-20, drawWidth/2,100);
        //ofRect(50,150, drawWidth/2-50,150);
        audioShader.end();
    }
}



void testApp::soundSetup()
{
//    soundStream.listDevices();
//    
//    //if you want to set a different device id
//    soundStream.setDeviceID(2); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
//    int bufferSize = 256;
//    left.assign(bufferSize*2, 0.0);
//    soundStream.setup(this, 0, 1, 44100, bufferSize, 1);
    
//    audioStrip.setAttributeData(audioAttribLocation, levels, 1,2*bufferSize, GL_DYNAMIC_DRAW);
}


void testApp::setupFlagVerts()
{
//    int bufferSize = 256;
    
    int vertsAcross = 100;
    int vertsDown = 100;
    int totalVerts = vertsAcross*vertsDown*6;
    ofVec3f* flagVerts = new ofVec3f[totalVerts];
    ofVec2f* flagTexCoords = new ofVec2f[totalVerts];
    float* flagTexCoordsX = new float[totalVerts*2];
    float* flagTexCoordsY = new float[totalVerts*2];
    
    float hSpacing = ofGetScreenWidth() *1.f/vertsAcross;;
    float vSpacing = ofGetScreenWidth() *1.f/vertsDown;;

    for(int j = 0; j < vertsDown; j++)
    {
        for(int i = 0; i < vertsAcross; i++)
        {
            int curIndex = 6*(i+j*vertsAcross);
            flagVerts[curIndex+0]     = ofVec3f(i*hSpacing, j*vSpacing,0);
            flagTexCoords[curIndex+0] = ofVec2f(i*1.f/vertsAcross,j*1.f/vertsDown);
            flagTexCoordsX[curIndex+0] =i*1.f/vertsAcross;
            flagTexCoordsY[curIndex+0] =j*1.f/vertsDown;
            
            flagVerts[curIndex+1]     = ofVec3f((i+1)*hSpacing, j*vSpacing,0);
            flagTexCoords[curIndex+1] = ofVec2f((i+1)*1.f/vertsAcross,j*1.f/vertsDown);
            flagTexCoordsX[curIndex+1] =(i+1)*1.f/vertsAcross;
            flagTexCoordsY[curIndex+1] =j*1.f/vertsDown;

            
            flagVerts[curIndex+2]     = ofVec3f((i+1)*hSpacing, (j+1)*vSpacing,0);
            flagTexCoords[curIndex+2] = ofVec2f((i+1)*1.f/vertsAcross,(j+1)*1.f/vertsDown);
            flagTexCoordsX[curIndex+2] =(i+1)*1.f/vertsAcross;
            flagTexCoordsY[curIndex+2] =(j+1)*1.f/vertsDown;
            
            flagVerts[curIndex+3]     = ofVec3f(i*hSpacing, j*vSpacing,0);
            flagTexCoords[curIndex+3] = ofVec2f(i*1.f/vertsAcross,j*1.f/vertsDown);
            flagTexCoordsX[curIndex+3] =i*1.f/vertsAcross;
            flagTexCoordsY[curIndex+3] =j*1.f/vertsDown;
            
            flagVerts[curIndex+4]     = ofVec3f((i+1)*hSpacing, (j+1)*vSpacing,0);
            flagTexCoords[curIndex+4] = ofVec2f((i+1)*1.f/vertsAcross,(j+1)*1.f/vertsDown);
            flagTexCoordsX[curIndex+4] =(i+1)*1.f/vertsAcross;
            flagTexCoordsY[curIndex+4] =(j+1)*1.f/vertsDown;
            
            flagVerts[curIndex+5]     = ofVec3f(i*hSpacing, (j+1)*vSpacing,0);
            flagTexCoords[curIndex+5] = ofVec2f(i*1.f/vertsAcross,(j+1)*1.f/vertsDown);
            flagTexCoordsX[curIndex+5] =i*1.f/vertsAcross;
            flagTexCoordsY[curIndex+5] =(j+1)*1.f/vertsDown;
        }
    }
    audioStrip.setVertexData(flagVerts, totalVerts, GL_STATIC_DRAW);
    audioStrip.setTexCoordData(flagTexCoords, totalVerts, GL_STATIC_DRAW);
    audioStrip.setAttributeData(audioXAttribLocation, flagTexCoordsX, 1,totalVerts, GL_STATIC_DRAW);
    audioStrip.setAttributeData(audioYAttribLocation, flagTexCoordsY, 1,totalVerts, GL_STATIC_DRAW);
    //clean the arrays up???
//    delete [] flagVerts;
//    delete [] flagTexCoords;
}

void testApp::audioIn(float * input, int bufferSize, int nChannels)
{
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++)
    {
        left[2*i] = left[2*i+1]  = input[i]*1800;
        levels[2*i+1] = levels[2*i] = left[2*i];
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    switch(key)
    {
        case 'f':
            ofToggleFullscreen();
            initSprites(); //re-init sprites with propersizing
        break;
        default:
        break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

