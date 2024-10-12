#pragma once

#include <GLCore.h>

struct AnimationFrame
{
    GLCore::Ref<GLCore::SubTexture2D> SubTexture;
    float TimeAfterFrame;

    AnimationFrame() = default;

    AnimationFrame(GLCore::Ref<GLCore::SubTexture2D> subTexture, float timeAfterFrame)
        : SubTexture(subTexture), TimeAfterFrame(timeAfterFrame)
    {
    }
};

class AnimationTopDown
{
public:
    AnimationTopDown(const char* name, bool loop);

    void AddFrame(const AnimationFrame& animationFrame);

    void OnUpdate(GLCore::Timestep timestep);
    
    void Play();
    void Stop();

    const char* GetName() const { return m_Name; }
private:
    const char* m_Name;
    bool m_Loop;
    std::vector<GLCore::Ref<AnimationFrame>> m_Frames;

    bool m_IsRunning = false;
    float m_TimeSinceLastFrame = 0.0f;
    int m_CurrentFrameIndex = 0;
};

class AnimatorTopDown
{
    AnimatorTopDown() = default;
    ~AnimatorTopDown() = default;

    void AddAnimation(const char* name, const AnimationTopDown& animation);

    void OnUpdate(GLCore::Timestep timestep);

    void SetActiveAnimation(const char* name);
private:
    std::unordered_map<const char*, GLCore::Ref<AnimationTopDown>> m_AnimationsMap;

    GLCore::Ref<AnimationTopDown> m_ActiveAnimation;
    // TODO - then in here we have a reference to a sprite component
};

