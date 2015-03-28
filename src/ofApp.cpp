#include "ofApp.h"

// comparison routine for sort...
bool sortVertically(  basicSprite * a, basicSprite * b ) {
    return a->pos.y > b->pos.y;
}

//--------------------------------------------------------------
void testApp::setup(){
    
    camWidth 		= 640;	// try to grab at this size.
    camHeight 		= 480;
    hCount = 100;
    vCount = 70;
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    ofSetFrameRate(30); //lets run at 30 fps!
    
    ofEnableAlphaBlending(); // turn on alpha blending. important!
    charToIndexMap.clear();
    charToIndexMap[' ']=0;
    charToIndexMap['!']=1;
    charToIndexMap['\"']=2;
    charToIndexMap['#']=3;
    charToIndexMap['$']=4;
    charToIndexMap['%']=5;
    charToIndexMap['&']=6;
    charToIndexMap['\'']=7;
    charToIndexMap['(']=8;
    charToIndexMap[')']=9;
    charToIndexMap['*']=10;
    charToIndexMap['+']=11;
    charToIndexMap['`']=12;
    charToIndexMap['-']=13;
    charToIndexMap['.']=14;
    charToIndexMap['/']=15;
    charToIndexMap['0']=16;
    charToIndexMap['1']=17;
    charToIndexMap['2']=18;
    charToIndexMap['3']=19;
    charToIndexMap['4']=20;
    charToIndexMap['5']=21;
    charToIndexMap['6']=22;
    charToIndexMap['7']=23;
    charToIndexMap['8']=24;
    charToIndexMap['9']=25;
    charToIndexMap[':']=26;
    charToIndexMap[';']=27;
    charToIndexMap['<']=28;
    charToIndexMap['=']=29;
    charToIndexMap['>']=30;
    charToIndexMap['?']=31;
    charToIndexMap['@']=32;
    charToIndexMap['A']=33;
    charToIndexMap['B']=34;
    charToIndexMap['C']=35;
    charToIndexMap['D']=36;
    charToIndexMap['E']=37;
    charToIndexMap['F']=38;
    charToIndexMap['G']=39;
    charToIndexMap['H']=40;
    charToIndexMap['I']=41;
    charToIndexMap['J']=42;
    charToIndexMap['K']=43;
    charToIndexMap['L']=44;
    charToIndexMap['M']=45;
    charToIndexMap['N']=46;
    charToIndexMap['O']=47;
    charToIndexMap['P']=48;
    charToIndexMap['Q']=49;
    charToIndexMap['R']=50;
    charToIndexMap['S']=51;
    charToIndexMap['T']=52;
    charToIndexMap['U']=53;
    charToIndexMap['V']=54;
    charToIndexMap['W']=55;
    charToIndexMap['X']=56;
    charToIndexMap['Y']=57;
    charToIndexMap['Z']=58;
    charToIndexMap['[']=59;
    charToIndexMap['\\']=60;
    charToIndexMap[']']=61;
    charToIndexMap['^']=62;
    charToIndexMap['_']=63;
//    charToIndexMap['\'']=64;
    charToIndexMap['a']=65;
    charToIndexMap['b']=66;
    charToIndexMap['c']=67;
    charToIndexMap['d']=68;
    charToIndexMap['e']=69;
    charToIndexMap['f']=70;
    charToIndexMap['g']=71;
    charToIndexMap['h']=72;
    charToIndexMap['i']=73;
    charToIndexMap['j']=74;
    charToIndexMap['k']=75;
    charToIndexMap['l']=76;
    charToIndexMap['m']=77;
    charToIndexMap['n']=78;
    charToIndexMap['o']=79;
    charToIndexMap['p']=80;
    charToIndexMap['q']=81;
    charToIndexMap['r']=82;
    charToIndexMap['s']=83;
    charToIndexMap['t']=84;
    charToIndexMap['u']=85;
    charToIndexMap['v']=86;
    charToIndexMap['w']=87;
    charToIndexMap['x']=88;
    charToIndexMap['y']=89;
    charToIndexMap['z']=90;
    charToIndexMap['{']=91;
    charToIndexMap['|']=92;
    charToIndexMap['}']=93;
    charToIndexMap['~']=94;
    
    initSprites();
//" !\"#$%&'()*+_@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_abcdefghijklmnopqrstuvwxyz{|}~"
}

void testApp::initSprites()
{
    
    //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
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
//            newSprite->speed=ofRandom(1,5); //set its speed
            
            //set its animation to the walk animation we declared
            newSprite->animation = walkAnimation;
            
            //adjust its frame duration based on how fast it is walking (faster = smaller)
            newSprite->animation.frame_duration /= newSprite->speed;
//            charToIndexMap.size()
            int curindex = (i+j*hCount)%s.length();//charToIndexMap.size();
            //change the start index of our sprite. we have 4 rows of animations and our spritesheet is 8 tiles wide, so our possible start indicies are 0, 8, 16, and 24
            newSprite->animation.index = charToIndexMap[s[curindex]];//(int)ofRandom(0,10)*ofRandom(0,10);
            sprites.push_back(newSprite); //add our sprite to the vector
        }
    }
    cout << "count: " << charToIndexMap.size() << endl;
}

//--------------------------------------------------------------
void testApp::update(){
    	vidGrabber.update();
    ofPixelsRef pixelsRef = vidGrabber.getPixelsRef();
    
    spriteRenderer->clear(); // clear the sheet
    spriteRenderer->update(ofGetElapsedTimeMillis()); //update the time in the renderer, this is necessary for animations to advance
    
    sort( sprites.begin(), sprites.end(), sortVertically ); // sorts the sprites vertically so the ones that are lower are drawn later and there for in front of the ones that are higher up
    
    if(sprites.size()>0) // if we have sprites
    {
        for(int i=sprites.size()-1;i>=0;i--) //go through them
        {
            ofVec2f movieCoord =ofVec2f(camWidth * sprites[i]->pos.x/ofGetWindowWidth(),
                                        camHeight * sprites[i]->pos.y/ofGetWindowHeight());
            float lightness = pixelsRef.getColor(movieCoord.x, movieCoord.y).getLightness()/ofColor().limit();
            int charIndex = (int)(lightness*charToIndexMap.size());
            sprites[i]->animation.index = charToIndexMap[charIndex];
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
void testApp::draw(){
    ofBackground(0);
    spriteRenderer->draw(); //draw the sprites!
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
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

