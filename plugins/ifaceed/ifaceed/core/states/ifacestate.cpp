#include "ifacestate.h"
#include "../ifaceeditor.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"

IFaceEditor * IFaceState::editor()
{
	return static_cast<IFaceEditor *>(this->behaviour()->parent());
}

EditorBehaviourSharedData * IFaceState::shdata()
{
	return this->editor()->behaviourSharedData();
}

