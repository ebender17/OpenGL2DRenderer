#include "AnimatorTopDown.h"

using namespace GLCore;

/*----------Animation----------*/

AnimationTopDown::AnimationTopDown(const char* name, bool loop)
    : m_Name(name), m_Loop(loop)
{
    m_Frames.reserve(4);
}

void AnimationTopDown::AddFrame(const AnimationFrame& animationFrame)
{
    m_Frames.emplace_back(animationFrame);
}

void AnimationTopDown::OnUpdate(Timestep timestep)
{
    if (!m_IsRunning) { return; }

    m_TimeSinceLastFrame += timestep;

    if (m_TimeSinceLastFrame > m_Frames[m_CurrentFrameIndex]->TimeAfterFrame) // time to change current frame
    {
        if ((m_CurrentFrameIndex + 1) < m_Frames.size()) // next index is valid
        {
            m_TimeSinceLastFrame = 0.0f;
            m_CurrentFrameIndex++;
        }
        else if ((m_CurrentFrameIndex + 1) > m_Frames.size() && m_Loop) // next index is not valid & loop
        {
            m_TimeSinceLastFrame = 0.0f;
            m_CurrentFrameIndex = 0;
        }
        else
        {
            Stop();
        }
    }

}

void AnimationTopDown::Play()
{
    m_IsRunning = true;
}

void AnimationTopDown::Stop()
{
    m_IsRunning = false;
    m_TimeSinceLastFrame = 0.0f;
    m_CurrentFrameIndex = 0;
}

/*----------Animator----------*/

void AnimatorTopDown::AddAnimation(const char* name, const AnimationTopDown& animation)
{
    if (m_AnimationsMap.find(name) != m_AnimationsMap.end())
    {
        GLCORE_ASSERT(false, "Animation name already exists!");
        return;
    }

    m_AnimationsMap[animation.GetName()] = CreateRef<AnimationTopDown>(animation);
}

void AnimatorTopDown::OnUpdate(Timestep timestep)
{
    if (m_ActiveAnimation == nullptr) { return; }
    m_ActiveAnimation->OnUpdate(timestep);
}

void AnimatorTopDown::SetActiveAnimation(const char* name)
{
    if (m_ActiveAnimation->GetName() == name) { return; }

    if (m_AnimationsMap.find(name) != m_AnimationsMap.end())
    {
        m_ActiveAnimation->Stop();
        m_ActiveAnimation = m_AnimationsMap[name];
        m_ActiveAnimation->Play();
    }
}
