/*! \file layoutschange.h


    Defines a generic change command for layouts
 */
#pragma once
#include "../command.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"
#include "../../gui/actions/scenenodeactions.h"


#include <3rdparty/picojson/picojson.h>

#include <sadvector.h>

#include <layouts/grid.h>

namespace history
{

namespace layouts
{
/*! Describes a generic change in layouts
 */
template<
    gui::actions::GridActions::UpdateOptions O
>
class Change: public history::Command
{
    /*! Creates new command for specified grid
        \param[in] g grid
     */
    Change(sad::layouts::Grid* g) 
    : m_grid(g), 
      m_old_state(picojson::object_type, false),
      m_new_state(picojson::object_type, false)
    {
        m_grid->addRef();        
    }
    /*! Destroys all linked data
     */
    virtual ~Change()
    {
        m_grid->delRef();
        for(size_t i = 0; i < m_affected_nodes.size(); i++)
        {
            m_affected_nodes[i]->delRef();
        }
    }
    /*! Adds new affected node to list
     */
    void addAffectedNode(sad::SceneNode* node)
    {
        node->addRef();
        m_affected_nodes << node;
    }
    /*! Saves old state for a grid
     */
    void saveOldState()
    {
        m_grid->save(m_old_state);
    }

    /*! Saves state to a new state for a grid
     */
    void saveNewState()
    {
        m_grid->save(m_new_state);
    }
     /*! Applies new saved state, described in command
         \param[in] ob an editor
      */
    virtual void commit(core::Editor * ob = NULL)
    {
       if (!ob)
       {
           return;
       }
       m_grid->load(m_new_state);
       tryUpdateUI(ob);
    }
     /*! Reverts to old saved state, describled in command
         \param[in] ob an editor
      */
    virtual void rollback(core::Editor * ob = NULL)
    {
        if (!ob)
        {
            return;
        }
       m_grid->load(m_old_state);
       tryUpdateUI(ob);
    }
protected:
    /*! Tries to update UI
        \param[in] e editor
     */
    void tryUpdateUI(core::Editor* e)
    {
        if (!e)
            return;
        if (e->shared()->selectedGrid() == m_grid)
        {
            e->actions()->gridActions()->updateGridPropertiesInUIExcept(O);
        }
        for(size_t i = 0; i < m_affected_nodes.size(); i++)
        {
            if (e->shared()->selectedObject() == m_affected_nodes[i])
            {
                e->actions()->sceneNodeActions()->updateUIForSelectedSceneNode();
            }
        }
    }
    /*! An affected grid
     */
    sad::layouts::Grid* m_grid;
    /*! A list of affected nodes by grid change
     */
    sad::Vector<sad::SceneNode*> m_affected_nodes;
    /*! An old state for layout change
     */
    picojson::value m_old_state;
    /*! A new state for layout change
     */
    picojson::value m_new_state;
};

}

}
