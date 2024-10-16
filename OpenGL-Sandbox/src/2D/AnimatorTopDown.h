#pragma once

#include <GLCore.h>

struct AnimationFrame
{
    GLCore::Ref<GLCore::SubTexture2D> SubTexture;
    float TimeAfterFrame = 0.0f;

    AnimationFrame() = default;

    AnimationFrame(GLCore::Ref<GLCore::SubTexture2D> subTexture, float timeAfterFrame)
        : SubTexture(subTexture), TimeAfterFrame(timeAfterFrame)
    {
    }
};

class AnimationTopDown
{
public:
    AnimationTopDown(const std::string& name, bool loop);

    void AddFrame(GLCore::Ref<AnimationFrame> animationFrame);

    void OnUpdate(GLCore::Timestep timestep);
    
    void Play();
    void Stop();

    const std::string& GetName() const { return m_Name; }
private:
    std::string m_Name; // TODO : Use enums instead of names?
    bool m_Loop;
    std::vector<GLCore::Ref<AnimationFrame>> m_Frames;

    bool m_IsRunning = false;
    float m_TimeSinceLastFrame = 0.0f;
    int m_CurrentFrameIndex = 0;
};

class AnimatorTopDown
{
public:
    AnimatorTopDown() = default;
    ~AnimatorTopDown() = default;

    void AddAnimation(GLCore::Ref<AnimationTopDown> animation);

    void OnUpdate(GLCore::Timestep timestep);

    void SetActiveAnimation(const std::string& name);
private:
    std::unordered_map<std::string, GLCore::Ref<AnimationTopDown>> m_AnimationsMap;

    GLCore::Ref<AnimationTopDown> m_ActiveAnimation;
    // TODO - then in here we have a reference to a sprite component
};

