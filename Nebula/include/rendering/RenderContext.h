//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <atomic>

#include "core/Types.h"
#include "rendering/commands/RenderCommandVisitor.h"

namespace nebula {

    namespace threads { class MainRenderThread; }

    namespace rendering {

        class Framebuffer;
        class FramebufferTemplate;

        class NEBULA_API RenderContext
        {
        public:
            virtual ~RenderContext();

            void setRenderFps(const uint32_t fps) { m_current_render_fps.store(fps); }
            [[nodiscard]] std::atomic_uint32_t getRenderFps() const { return m_current_render_fps.load(); }
            [[nodiscard]] std::atomic_uint32_t getCurrentRenderFrame() const { return m_current_render_frame.load(); }
            [[nodiscard]] uint32_t getFramesInFlightNumber() const { return m_frames_in_flight_number; }

            static RenderContext& get() { return *s_instance; }

        protected:
            RenderContext();    //  Defined in MainRenderThread

            virtual bool checkVSync() = 0;
            virtual void setVSync(bool vsync) = 0;

        private:
            uint32_t m_frames_in_flight_number;
            std::atomic_uint32_t m_current_render_frame;
            std::atomic_uint32_t m_current_render_fps = 60;

            static RenderContext* s_instance;

            //  Called by RenderGraphThread
            virtual void waitForFrameResources(uint32_t frame) = 0;

            //  Called by MainRenderThread
            virtual void bind() = 0;
            virtual void unbind() = 0;

            virtual void presentImage() = 0;
            virtual Reference<Framebuffer> getNextImage() = 0;

            virtual Scope<ExecuteCommandVisitor> getCommandExecutor() = 0; 

            [[nodiscard]] virtual const Reference<FramebufferTemplate>& viewFramebufferTemplate() const = 0;

            friend class nebula::threads::MainRenderThread;
            static Scope<RenderContext> create(void* window_handle);
        };

    }

}

#endif //RENDERCONTEXT_H
