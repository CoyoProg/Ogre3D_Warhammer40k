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
		OverlayContainer* Mainpanel;
		olm = OverlayManager::getSingletonPtr();

		/* Create the Main Panel */
		Mainpanel = static_cast<OverlayContainer*>(olm->createOverlayElement("Panel", "MainPanel"));
		Mainpanel->setMetricsMode(Ogre::GMM_RELATIVE);
		Mainpanel->setPosition(0.0f, 0.0f);
		Mainpanel->setDimensions(1.f, 1.f);

		/* Create the Players turn text element */
		playerTurnsText = static_cast<TextAreaOverlayElement*>(olm->createOverlayElement("TextArea", "PlayersTurnText"));
		playerTurnsText->setCaption("PlayerOne");
		playerTurnsText->setMetricsMode(Ogre::GMM_RELATIVE);
		playerTurnsText->setPosition(0.5f, 0.9f); // At the bottom of the screen
		playerTurnsText->setDimensions(1.0f, 1.0f);
		playerTurnsText->setAlignment(TextAreaOverlayElement::Center);
		playerTurnsText->setFontName("MyFont");
		playerTurnsText->setCharHeight(0.08f);
		playerTurnsText->setColour(Ogre::ColourValue(0.0f, 0.f, 0.7f, 1.f));
		Mainpanel->addChild(playerTurnsText);


		float cardWidth = 250.f * 1.25f;
		float cardHeight = 300.f * 1.25f;
		/* Create the Left Card Element */
		leftCard = static_cast<PanelOverlayElement*>(olm->createOverlayElement("Panel", "LeftCard"));
		leftCard->setMetricsMode(GMM_PIXELS);
		leftCard->setPosition(50.f, 450.f);
		leftCard->setDimensions(cardWidth, cardHeight);
		leftCard->setMaterialName("cardOverlay");
		leftCard->hide();
		Mainpanel->addChild(leftCard);

		/* Create the Left Card TEXT element */
		LeftCardText = static_cast<TextAreaOverlayElement*>(olm->createOverlayElement("TextArea", "LeftCardText"));
		LeftCardText->setCaption(std::string("Health Point: 5 \n\n") + "Movement Point : 10");
		LeftCardText->setMetricsMode(Ogre::GMM_PIXELS);
		LeftCardText->setPosition(50.f, cardHeight / 2 + 75);
		LeftCardText->setDimensions(100.f, 100.f);
		LeftCardText->setAlignment(TextAreaOverlayElement::Left);
		LeftCardText->setFontName("MyFont");
		LeftCardText->setCharHeight(24.f);
		LeftCardText->setColour(Ogre::ColourValue::Black);
		leftCard->addChild(LeftCardText);

		LeftCardNameText = static_cast<TextAreaOverlayElement*>(olm->createOverlayElement("TextArea", "LeftCardNameText"));
		LeftCardNameText->setCaption("FIGURINE'S NAME");
		LeftCardNameText->setMetricsMode(Ogre::GMM_PIXELS);
		LeftCardNameText->setPosition(cardWidth / 2, cardHeight / 2 + 35);
		LeftCardNameText->setDimensions(100.f, 100.f);
		LeftCardNameText->setAlignment(TextAreaOverlayElement::Center);
		LeftCardNameText->setFontName("MyFont");
		LeftCardNameText->setCharHeight(24.f);
		LeftCardNameText->setColour(Ogre::ColourValue::Black);
		leftCard->addChild(LeftCardNameText);

		/* Create the Right Card Element */
		rightCard = static_cast<PanelOverlayElement*>(olm->createOverlayElement("Panel", "RightCard"));
		rightCard->setMetricsMode(GMM_PIXELS);
		rightCard->setPosition(1180.f, 450.f);
		rightCard->setDimensions(cardWidth, cardHeight);
		rightCard->setMaterialName("cardOverlay");
		rightCard->hide();
		Mainpanel->addChild(rightCard);

		/* Create the Right Card TEXT element */
		RightCardText = static_cast<TextAreaOverlayElement*>(olm->createOverlayElement("TextArea", "RightCardText"));
		RightCardText->setCaption(std::string("Health Point: 5 \n\n") + "Movement Point : 10");
		RightCardText->setMetricsMode(Ogre::GMM_PIXELS);
		RightCardText->setPosition(50.f, cardHeight / 2 + 75);
		RightCardText->setDimensions(100.f, 100.f);
		RightCardText->setAlignment(TextAreaOverlayElement::Left);
		RightCardText->setFontName("MyFont");
		RightCardText->setCharHeight(24.f);
		RightCardText->setColour(Ogre::ColourValue::Black);
		rightCard->addChild(RightCardText);

		RightCardNameText = static_cast<TextAreaOverlayElement*>(olm->createOverlayElement("TextArea", "RightCardNameText"));
		RightCardNameText->setCaption("FIGURINE'S NAME");
		RightCardNameText->setMetricsMode(Ogre::GMM_PIXELS);
		RightCardNameText->setPosition(cardWidth / 2, cardHeight / 2 + 35);
		RightCardNameText->setDimensions(100.f, 100.f);
		RightCardNameText->setAlignment(TextAreaOverlayElement::Center);
		RightCardNameText->setFontName("MyFont");
		RightCardNameText->setCharHeight(24.f);
		RightCardNameText->setColour(Ogre::ColourValue::Black);
		rightCard->addChild(RightCardNameText);
		
		
		overlay = olm->create("GUI_OVERLAY");
		overlay->add2D(Mainpanel);


		//overlay = olm->getByName("GUI_OVERLAY");

		overlay->show();
	}

	~OgreText()
	{
		olm->destroyAllOverlayElements();
		olm->destroy("GUI_OVERLAY");
	}

	void setCol(float R, float G, float B, float I)
	{
		playerTurnsText->setColour(Ogre::ColourValue(R, G, B, I));
	}

	TextAreaOverlayElement* GetPlayerTextElement() { return playerTurnsText; }
	PanelOverlayElement* GetLeftCardElement() { return leftCard; }
	PanelOverlayElement* GetRightCardElement() { return rightCard; }

private:
	/* Overlays */
	OverlayManager* olm;
	Overlay* overlay;

	/* Overlay Elements*/
	PanelOverlayElement* leftCard;
	PanelOverlayElement* rightCard;
	TextAreaOverlayElement* RightCardText;
	TextAreaOverlayElement* RightCardNameText;
	TextAreaOverlayElement* playerTurnsText;
	TextAreaOverlayElement* LeftCardText;
	TextAreaOverlayElement* LeftCardNameText;


	String szElement;
};

#endif