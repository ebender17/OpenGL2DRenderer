#pragma once

#include <GLCore.h>

struct AnimationFrame
{
    GLCore::Ref<GLCore::SubTexture2D> SubTexture;
    float FrameDuration = 0.0f;

    AnimationFrame() = default;

    AnimationFrame(GLCore::Ref<GLCore::SubTexture2D> subTexture, float frameDuration)
        : SubTexture(subTexture), FrameDuration(frameDuration)
    {
    }
};

class AnimationTopDown
{
public:
    AnimationTopDown(const char* name, bool loop);

    void AddFrame(GLCore::Ref<AnimationFrame> animationFrame);

    void OnUpdate(GLCore::Timestep timestep);
    
    void Play();
    void Stop();

    void SetAnimationStopCallback(const std::function<void()>& callback) { m_OnAnimationStop = callback; }

    const char* GetName() const { return m_Name; }
    GLCore::Ref<AnimationFrame> GetCurrentFrame() { return m_Frames[m_CurrentFrameIndex]; }
private:
    const char* m_Name; // TODO : Use enums instead of names?
    bool m_Loop;
    std::vector<GLCore::Ref<AnimationFrame>> m_Frames;
    std::function<void()> m_OnAnimationStop;

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

    void SetActiveAnimation(const char* name);

    GLCore::Ref<AnimationFrame> GetCurrentFrame() { return m_ActiveAnimation->GetCurrentFrame(); }
private:
    std::unordered_map<const char*, GLCore::Ref<AnimationTopDown>> m_AnimationsMap;

    GLCore::Ref<AnimationTopDown> m_ActiveAnimation;
};