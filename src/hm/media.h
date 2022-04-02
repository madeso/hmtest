#pragma once


class Engine;


class Media
{
public:
    Media(Engine* pEngine, bool pHardware);
    virtual ~Media();


    virtual void
    loadMedia() = 0;

    virtual void
    unloadMedia() = 0;

    bool
    isLoaded() const;

    bool
    isHardwareDependent() const;

    void
    load();

    void
    unload();

    Engine&
    getEngine();

private:
    bool loaded;
    bool hardware;
    Engine* engine;
};

