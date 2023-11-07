#pragma once

class OgreSetup
{
public:
	// This singleton will ensure the existence and unicity of the game instance
	static OgreSetup& instance()
	{
		static OgreSetup inst;
		return inst;
	}

	OgreSetup() {};
	OgreSetup(const OgreSetup&) = delete;
	OgreSetup& operator = (const OgreSetup&) = delete;
	OgreSetup(OgreSetup&&) = delete;
	OgreSetup& operator = (OgreSetup&&) = delete;

	void SetupEngine();
	void Play() {};

private:
};

