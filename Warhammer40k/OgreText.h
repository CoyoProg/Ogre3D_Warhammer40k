//-----------------------------------------------------------------------------
// Lonewolff
//
// Filename:    OgreText.h
// Description: Class for simple text in Ogre (Version 040507:18.30)
//-----------------------------------------------------------------------------

#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>

#include "OgreTextAreaOverlayElement.h"
#include "OgrePanelOverlayElement.h"
#include "OgreStringConverter.h"

using namespace Ogre;
#include <iostream>
#ifndef __OgreText_H__
#define __OgreText_H__

class OgreText
{
public:
	OgreText()
	{
		OverlayContainer *Mainpanel;
		mOverlayManager = OverlayManager::getSingletonPtr();

		/* Create the Main Panel */
		Mainpanel = static_cast<OverlayContainer*>(mOverlayManager->createOverlayElement("Panel", "MainPanel"));
		Mainpanel->setMetricsMode(Ogre::GMM_RELATIVE);
		Mainpanel->setPosition(0.0f, 0.0f);
		Mainpanel->setDimensions(1.f, 1.f);

		/* Create the Players turn text element */
		mPlayerTurnsText = static_cast<TextAreaOverlayElement*>(mOverlayManager->createOverlayElement("TextArea", "PlayersTurnText"));
		mPlayerTurnsText->setCaption("PlayerOne");
		mPlayerTurnsText->setMetricsMode(Ogre::GMM_RELATIVE);
		mPlayerTurnsText->setPosition(0.5f, 0.9f); // At the bottom of the screen
		mPlayerTurnsText->setDimensions(1.0f, 1.0f);
		mPlayerTurnsText->setAlignment(TextAreaOverlayElement::Center);
		mPlayerTurnsText->setFontName("MyFont");
		mPlayerTurnsText->setCharHeight(0.08f);
		mPlayerTurnsText->setColour(Ogre::ColourValue(0.0f, 0.f, 0.7f, 1.f));
		Mainpanel->addChild(mPlayerTurnsText);


		float cardWidth = 250.f * 1.25f;
		float cardHeight = 300.f * 1.25f;
		/* Create the Left Card Element */
		mLeftCard = static_cast<PanelOverlayElement*>(mOverlayManager->createOverlayElement("Panel", "LeftCard"));
		mLeftCard->setMetricsMode(GMM_PIXELS);
		mLeftCard->setPosition(50.f, 450.f);
		mLeftCard->setDimensions(cardWidth, cardHeight);
		mLeftCard->setMaterialName("cardOverlay");
		mLeftCard->hide();
		Mainpanel->addChild(mLeftCard);

		/* Create the Left Card TEXT element */
		mLeftCardText = static_cast<TextAreaOverlayElement*>(mOverlayManager->createOverlayElement("TextArea", "LeftCardText"));
		mLeftCardText->setCaption(std::string("Health Point: 5 \n\n") + "Movement Point : 10");
		mLeftCardText->setMetricsMode(Ogre::GMM_PIXELS);
		mLeftCardText->setPosition(50.f, cardHeight / 2 + 75);
		mLeftCardText->setDimensions(100.f, 100.f);
		mLeftCardText->setAlignment(TextAreaOverlayElement::Left);
		mLeftCardText->setFontName("MyFont");
		mLeftCardText->setCharHeight(24.f);
		mLeftCardText->setColour(Ogre::ColourValue::Black);
		mLeftCard->addChild(mLeftCardText);

		mLeftCardNameText = static_cast<TextAreaOverlayElement*>(mOverlayManager->createOverlayElement("TextArea", "LeftCardNameText"));
		mLeftCardNameText->setCaption("FIGURINE'S NAME");
		mLeftCardNameText->setMetricsMode(Ogre::GMM_PIXELS);
		mLeftCardNameText->setPosition(cardWidth / 2, cardHeight / 2 + 35);
		mLeftCardNameText->setDimensions(100.f, 100.f);
		mLeftCardNameText->setAlignment(TextAreaOverlayElement::Center);
		mLeftCardNameText->setFontName("MyFont");
		mLeftCardNameText->setCharHeight(24.f);
		mLeftCardNameText->setColour(Ogre::ColourValue::Black);
		mLeftCard->addChild(mLeftCardNameText);

		/* Create the Right Card Element */
		mRightCard = static_cast<PanelOverlayElement*>(mOverlayManager->createOverlayElement("Panel", "RightCard"));
		mRightCard->setMetricsMode(GMM_PIXELS);
		mRightCard->setPosition(1180.f, 450.f);
		mRightCard->setDimensions(cardWidth, cardHeight);
		mRightCard->setMaterialName("cardOverlay");
		mRightCard->hide();
		Mainpanel->addChild(mRightCard);

		/* Create the Right Card TEXT element */
		mRightCardText = static_cast<TextAreaOverlayElement*>(mOverlayManager->createOverlayElement("TextArea", "RightCardText"));
		mRightCardText->setCaption(std::string("Health Point: 5 \n\n") + "Movement Point : 10");
		mRightCardText->setMetricsMode(Ogre::GMM_PIXELS);
		mRightCardText->setPosition(50.f, cardHeight / 2 + 75);
		mRightCardText->setDimensions(100.f, 100.f);
		mRightCardText->setAlignment(TextAreaOverlayElement::Left);
		mRightCardText->setFontName("MyFont");
		mRightCardText->setCharHeight(24.f);
		mRightCardText->setColour(Ogre::ColourValue::Black);
		mRightCard->addChild(mRightCardText);

		mRightCardNameText = static_cast<TextAreaOverlayElement*>(mOverlayManager->createOverlayElement("TextArea", "RightCardNameText"));
		mRightCardNameText->setCaption("FIGURINE'S NAME");
		mRightCardNameText->setMetricsMode(Ogre::GMM_PIXELS);
		mRightCardNameText->setPosition(cardWidth / 2, cardHeight / 2 + 35);
		mRightCardNameText->setDimensions(100.f, 100.f);
		mRightCardNameText->setAlignment(TextAreaOverlayElement::Center);
		mRightCardNameText->setFontName("MyFont");
		mRightCardNameText->setCharHeight(24.f);
		mRightCardNameText->setColour(Ogre::ColourValue::Black);
		mRightCard->addChild(mRightCardNameText);
		
		
		mOverlay = mOverlayManager->create("GUI_OVERLAY");
		mOverlay->add2D(Mainpanel);


		//overlay = olm->getByName("GUI_OVERLAY");

		mOverlay->show();
	}

	~OgreText()
	{
		mOverlayManager->destroyAllOverlayElements();
		mOverlayManager->destroy("GUI_OVERLAY");
	}

	void setCol(float R, float G, float B, float I)
	{
		mPlayerTurnsText->setColour(Ogre::ColourValue(R, G, B, I));
	}

	TextAreaOverlayElement* GetPlayerTextElement() { return mPlayerTurnsText; }
	PanelOverlayElement* GetLeftCardElement() { return mLeftCard; }
	PanelOverlayElement* GetRightCardElement() { return mRightCard; }

private:
	/* Overlays */
	OverlayManager *mOverlayManager;
	Overlay *mOverlay;

	/* Overlay Elements*/
	PanelOverlayElement *mLeftCard;
	PanelOverlayElement *mRightCard;
	TextAreaOverlayElement *mRightCardText;
	TextAreaOverlayElement *mRightCardNameText;
	TextAreaOverlayElement *mPlayerTurnsText;
	TextAreaOverlayElement *mLeftCardText;
	TextAreaOverlayElement *mLeftCardNameText;
};

#endif