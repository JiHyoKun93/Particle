/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

//http://www.effecthub.com/particle2dx

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

bool HelloWorld::init() {
    if (!Scene::init()) return false;

    //system = ParticleFire::create();
    //system->setPosition(1280 / 2 - 300, 720 / 2);
    //system->setRotation(90);
    //system->setRotationIsDir(false);
    //addChild(system);
    addChild(dn = DrawNode::create());
    addChild(dnGrid = DrawNode::create());

    { // Particle 부분
        system = ParticleMeteor::create();
        system->setPosition(CENTER.x - 300, CENTER.y);
        addChild(system);

        
        dn->drawRect(Vec2(30, 100), Vec2(500, 690), Color4F::MAGENTA);
    }
    {// Stats Grid 부분
        slider = Slider::create();
        slider->loadBarTexture("res/Slider_Back.png"); // what the slider looks like
        slider->loadSlidBallTextures("res/SliderNode_Normal.png", "res/SliderNode_Press.png", "res/SliderNode_Disable.png");
        slider->loadProgressBarTexture("res/Slider_PressBar.png");
        slider->setPosition(Vec2(CENTER.x - 300, 50));
        addChild(slider);

        Label* lbSlider = createLabel("Grid", Vec2(150, 50), 24.0f, Color3B::BLUE);
        
        slider->addEventListener([=](Ref* r, Slider::EventType e)->void {
            if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            auto percent = slider->getPercent();
            if (percent <= 1) return;
            drawGrid(percent);
            });
    }
    
    // 밑에서부터 Gravity Radius 윗 부분
    Label* lb = createLabel("Gravity Radius", Vec2(CENTER.x - 100, CENTER.y + 300), 24.0f, Color3B::BLUE);
    dn->drawLine(Vec2(CENTER.x - 100, CENTER.y + 280), Vec2(1250, CENTER.y + 280), Color4F::WHITE);

    {// Max Particles
        Slider* maxParticles = createSlider(Vec2(CENTER.x + 100, CENTER.y + 250));
        maxParticles->setMaxPercent(1991);
        maxParticles->setPercent(289);
        Label* lbParticles = createLabel("Max Particles", Vec2(CENTER.x - 100, CENTER.y + 250), 15.0f, Color3B::WHITE);
        maxParticles->addEventListener([=](Ref* r, Slider::EventType e)->void {
            if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            auto percent = maxParticles->getPercent();
            if (percent <= 1) return;
            system->setTotalParticles(percent);
        });
    }

    {// Duration
        Slider* duration = createSlider(Vec2(CENTER.x + 100, CENTER.y + 225));
        duration->setMaxPercent(10);
        Label* lbParticles = createLabel("Duration", Vec2(CENTER.x - 70, CENTER.y + 225), 15.0f, Color3B::WHITE);
        duration->addEventListener([=](Ref* r, Slider::EventType e)->void {
            if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            auto percent = (float)duration->getPercent();
            if (percent <= 1) percent = -1;
            system->setDuration(percent);
            });
    }

    {// LifeTime
        Slider* lifeTime = createSlider(Vec2(CENTER.x + 100, CENTER.y + 200));
        Label* lbParticles = createLabel("Life Time", Vec2(CENTER.x - 70, CENTER.y + 200), 15.0f, Color3B::WHITE);
        lifeTime->addEventListener([=](Ref* r, Slider::EventType e)->void {
            if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            auto percent = (float)lifeTime->getPercent() / 10;
            if (percent <= 0) return;
            system->setLife(percent);
            });
    }

    {// EmissionRate
        Slider* emissionRate = createSlider(Vec2(CENTER.x + 100, CENTER.y + 175));
        emissionRate->setMaxPercent(800);
        Label* lbParticles = createLabel("EmissionRate", Vec2(CENTER.x - 100, CENTER.y + 175), 15.0f, Color3B::WHITE);
        emissionRate->addEventListener([=](Ref* r, Slider::EventType e)->void {
            if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            auto percent = emissionRate->getPercent();
            if (percent <= 1) return;
            system->setEmissionRate(percent);
            });
    }

    {// Angle
        Slider* angle = createSlider(Vec2(CENTER.x + 100, CENTER.y + 150));
        angle->setMaxPercent(360);
        Label* lbParticles = createLabel("Angle", Vec2(CENTER.x - 50, CENTER.y + 150), 15.0f, Color3B::WHITE);
        angle->addEventListener([=](Ref* r, Slider::EventType e)->void {
            if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            auto percent = ((float)angle->getPercent() - 180);
            system->setAngle(percent);
        });
    }
    
    // 여기서부터는 아래부분
    

    {// Speed
        Slider* speed1 = createSlider(Vec2(CENTER.x + 100, CENTER.y - 150));
        Slider* speed2 = createSlider(Vec2(CENTER.x + 500, CENTER.y - 150));
        Label* lbParticles = createLabel("Speed", Vec2(CENTER.x - 50, CENTER.y - 150), 15.0f, Color3B::WHITE);

        speed1->addEventListener([=](Ref* r, Slider::EventType e)->void {
            if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
            auto percent = ((float)speed1->getPercent());
            system->setSpeed(percent);
            });
    }

    {// PosVar
        Slider* posVar1 = createSlider(Vec2(CENTER.x + 100, CENTER.y - 175));
        Slider* posVar2 = createSlider(Vec2(CENTER.x + 500, CENTER.y - 175));
        Label* lbParticles = createLabel("PosVar", Vec2(CENTER.x - 50, CENTER.y - 175), 15.0f, Color3B::WHITE);
    }

    {// GravityXY
        Slider* gravityXY1 = createSlider(Vec2(CENTER.x + 100, CENTER.y - 200));
        Slider* gravityXY2 = createSlider(Vec2(CENTER.x + 500, CENTER.y - 200));
        Label* lbParticles = createLabel("GravityXY", Vec2(CENTER.x - 50, CENTER.y - 200), 15.0f, Color3B::WHITE);
    }

    {// AccelRad
        Slider* accelRad1 = createSlider(Vec2(CENTER.x + 100, CENTER.y - 225));
        Slider* accelRad2 = createSlider(Vec2(CENTER.x + 500, CENTER.y - 225));
        Label* lbParticles = createLabel("AccelRad", Vec2(CENTER.x - 50, CENTER.y - 225), 15.0f, Color3B::WHITE);
    }

    {// AccelTan
        Slider* accelTan1 = createSlider(Vec2(CENTER.x + 100, CENTER.y - 250));
        Slider* accelTan2 = createSlider(Vec2(CENTER.x + 500, CENTER.y - 250));
        Label* lbParticles = createLabel("AccelTan", Vec2(CENTER.x - 50, CENTER.y - 250), 15.0f, Color3B::WHITE);
    }

    auto mouse = EventListenerMouse::create();
    mouse->onMouseDown = std::bind(&HelloWorld::onMouseDown, this, std::placeholders::_1);

    mouse->onMouseMove = std::bind(&HelloWorld::onMouseMove, this, std::placeholders::_1);

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, this);

    //schedule(CC_SCHEDULE_SELECTOR(HelloWorld::logic), 5.0f);

    return true;
}

void HelloWorld::onMouseDown(EventMouse* event) {
    auto pos = event->getLocationInView();
    if (pos.x >= 30 && pos.x <= 500 && pos.y >= 100 && pos.y <= 690) {
        system->setPosition(pos);
    }
}

void HelloWorld::onMouseMove(EventMouse* event){
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        auto pos = event->getLocationInView();
        if (pos.x >= 30 && pos.x <= 500 && pos.y >= 100 && pos.y <= 690) {
            system->setPosition(pos);
        }
    }
}

void HelloWorld::drawGrid(int size){
    dnGrid->clear();
    for (int i = 0; i < 470 / size + 1; i++) {
        float x = size * i;
        dnGrid->drawLine(Vec2(x + 30, 100), Vec2(x + 30, 690), Color4F::WHITE);
    }
    for (int i = 0; i < 590 / size + 1; i++) {
        float y = size * i;
        dnGrid->drawLine(Vec2(30, y + 100), Vec2(500, y + 100), Color4F::WHITE);
    }
}

Node* HelloWorld::createSliderUISet(const string& labelTitle, float valueMin, float valueMax, float initialValue, const function<void(float)> targetFunc) {
    Node* set = Node::create();
    
    Label* lb = Label::createWithTTF(labelTitle, "fonts/arial.ttf", 24.0f);
    lb->setPosition(Vec2(0, 20));
    set->addChild(lb);

    Slider* slider = Slider::create();
    slider->loadBarTexture("res/Slider_Back.png");
    slider->loadSlidBallTextures("res/SliderNode_Normal.png", "res/SliderNode_Press.png", "res/SliderNode_Disable.png");
    slider->loadProgressBarTexture("res/Slider_PressBar.png");
    slider->setPosition(Vec2(0, -20));
    addChild(set);
    set->addChild(slider);

    targetFunc(initialValue);
    slider->setPercent(initialValue);
    slider->setMaxPercent(valueMax - valueMin);

    slider->addEventListener([=](Ref* r, Slider::EventType e)->void {
        if (e != Slider::EventType::ON_PERCENTAGE_CHANGED) return;
        auto percent = slider->getPercent() + valueMin;
        
        targetFunc(percent);

        });

    return nullptr;
}

Slider* HelloWorld::createSlider(const Vec2& position){
    Slider* slider = Slider::create();
    slider->loadBarTexture("res/Slider_Back.png");
    slider->loadSlidBallTextures("res/SliderNode_Normal.png", "res/SliderNode_Press.png", "res/SliderNode_Disable.png");
    slider->loadProgressBarTexture("res/Slider_PressBar.png");
    slider->setPosition(Vec2(position));
    slider->setScale(0.7);
    
    addChild(slider);

    return slider;
}

Label* HelloWorld::createLabel(const string& text, const Vec2& position, float fontSize, Color3B color) {
    Label* lb = Label::createWithTTF(text, "fonts/arial.ttf", fontSize);
    lb->setAnchorPoint(Vec2(0, 0.5));
    lb->setPosition(position);
    lb->setColor(color);
    addChild(lb);
    return lb;
}

void HelloWorld::logic(float dt){
    system->setPosition(system->getPosition());
    system->setTotalParticles(system->getTotalParticles());
    system->setDuration(system->getDuration());
    system->setAngle(system->getAngle());
    system->setLife(system->getLife());
    system->setEmissionRate(system->getEmissionRate());
}

