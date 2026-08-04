#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// 1. THE ROBLOX PAUSE MENU POPUP
class RobloxMenu : public Popup<> {
protected:
    bool setup() override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        m_bgSprite->setColor({30, 30, 30}); 
        m_bgSprite->setOpacity(220);        

        auto topNavMenu = CCMenu::create();
        topNavMenu->setLayout(
            RowLayout::create()
                ->setGap(15.f)
                ->setAxisAlignment(AxisAlignment::Center)
        );
        topNavMenu->setPosition({winSize.width / 2, winSize.height - 40.f});
        this->addChild(topNavMenu);

        auto resumeTab = CCLabelBMFont::create("Resume", "bigFont.fnt");
        resumeTab->setScale(0.5f);
        auto resumeBtn = CCMenuItemSpriteExtra::create(resumeTab, this, menu_selector(RobloxMenu::onClose));
        topNavMenu->addChild(resumeBtn);

        topNavMenu->updateLayout();

        auto contentMenu = CCMenu::create();
        contentMenu->setLayout(
            ColumnLayout::create()
                ->setGap(10.f)
                ->setAxisAlignment(AxisAlignment::Center)
        );
        contentMenu->setPosition(winSize / 2);
        this->addChild(contentMenu);

        auto leaveLabel = CCLabelBMFont::create("Leave Game", "bigFont.fnt");
        leaveLabel->setScale(0.6f);
        auto leaveBtn = CCMenuItemSpriteExtra::create(leaveLabel, this, menu_selector(RobloxMenu::onLeaveGame));
        contentMenu->addChild(leaveBtn);

        contentMenu->updateLayout();
        return true;
    }

    void onLeaveGame(CCObject*) {
        auto playLayer = PlayLayer::get();
        if (playLayer) {
            playLayer->onQuit();
        }
    }

public:
    static RobloxMenu* create() {
        auto ret = new RobloxMenu();
        if (ret && ret->initAnchored(420.f, 260.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// 2. HOOKING THE PAUSE SCREEN
class $modify(MyPauseHook, PauseLayer) {
    bool init() {
        if (!PauseLayer::init()) return false;
        
        this->setVisible(false); 

        auto robloxUI = RobloxMenu::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(robloxUI, 100);

        return true;
    }
};

// 3. HOOKING THE HOME SCREEN
class $modify(RobloxHomeHook, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (auto mainButtonMenu = this->getChildByID("main-menu")) mainButtonMenu->setVisible(false);
        if (auto profileMenu = this->getChildByID("profile-menu")) profileMenu->setVisible(false);
        if (auto socialMenu = this->getChildByID("social-media-menu")) socialMenu->setVisible(false);
        if (auto bottomMenu = this->getChildByID("bottom-menu")) bottomMenu->setVisible(false);

        auto sidePanelBg = CCScale9Sprite::create("square02_001.png"); 
        sidePanelBg->setContentSize({140.f, winSize.height});
        sidePanelBg->setPosition({70.f, winSize.height / 2.f});
        sidePanelBg->setColor({25, 25, 25}); 
        sidePanelBg->setOpacity(180);         
        this->addChild(sidePanelBg, -1);      

        auto sidebarMenu = CCMenu::create();
        sidebarMenu->setLayout(
            ColumnLayout::create()
                ->setGap(12.f)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setCrossAxisLineAlignment(AxisAlignment::Start)
        );
        sidebarMenu->setPosition({50.f, winSize.height / 2.f});
        sidebarMenu->setContentSize({120.f, winSize.height - 80.f});
        this->addChild(sidebarMenu);

        auto homeLabel = CCLabelBMFont::create("Home", "bigFont.fnt");
        homeLabel->setScale(0.5f);
        auto homeBtn = CCMenuItemSpriteExtra::create(homeLabel, this, nullptr);
        sidebarMenu->addChild(homeBtn);

        auto playLabel = CCLabelBMFont::create("Discover", "bigFont.fnt");
        playLabel->setScale(0.5f);
        auto playBtn = CCMenuItemSpriteExtra::create(playLabel, this, menu_selector(MenuLayer::onPlay));
        sidebarMenu->addChild(playBtn);

        auto createLabel = CCLabelBMFont::create("Create", "bigFont.fnt");
        createLabel->setScale(0.5f);
        auto createBtn = CCMenuItemSpriteExtra::create(createLabel, this, menu_selector(MenuLayer::onCreator));
        sidebarMenu->addChild(createBtn);

        sidebarMenu->updateLayout();
        return true;
    }
};
