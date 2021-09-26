//-----------------------------------------
// A dialog to modify messh settings
//-----------------------------------------

#pragma once

#include "gApp.h"

#ifdef UI_WITH_MESH

    class dlgMeshSettings: public uiWindow
    {
        public:

            dlgMeshSettings();

        private:

            virtual void begin() override;
            virtual void update() override;
            virtual bool isModal() const override { return true; }
            virtual void onButton(const uiElement *ele, bool pressed) override;

    };


    extern dlgMeshSettings dlg_mesh_settings;

#endif