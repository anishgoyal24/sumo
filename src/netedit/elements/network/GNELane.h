/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2020 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    GNELane.h
/// @author  Jakob Erdmann
/// @date    Feb 2011
///
// A class for visualizing Lane geometry (adapted from GUILaneWrapper)
/****************************************************************************/
#pragma once
#include <config.h>

#include <netedit/elements/GNECandidateElement.h>

#include "GNENetworkElement.h"

// ===========================================================================
// class declarations
// ===========================================================================
class GUIGLObjectPopupMenu;
class PositionVector;
class GNETLSEditorFrame;
class GNEEdge;
class GNENet;
class GNEConnection;

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GNELane
 * @brief This lane is powered by an underlying GNEEdge and basically knows how
 * to draw itself
 */
class GNELane : public GNENetworkElement, public GNECandidateElement, public FXDelegator {
    /// @brief FOX-declaration
    FXDECLARE(GNELane)

public:
    /// @brief class for LaneDrawingConstants
    class LaneDrawingConstants {

    public:
        /// @brief parameter constructor
        LaneDrawingConstants(const GUIVisualizationSettings& s, const GNELane* lane);

        /// @brief selection scale
        const double selectionScale;

        /// @brief exaggeration
        const double exaggeration;

        // compute lane-marking intersection points)
        const double halfWidth2;

        /// @brief Draw as a normal lane, and reduce width to make sure that a selected edge can still be seen
        const double halfWidth;

    private:
        /// @brief default constructor
        LaneDrawingConstants();

        /// @brief Invalidated assignment operator.
        LaneDrawingConstants& operator=(const LaneDrawingConstants&) = delete;
    };

    /**@brief Constructor
     * @param[in] idStorage The storage of gl-ids to get the one for this lane representation from
     * @param[in] the edge this lane belongs to
     * @param[in] the index of this lane
     */
    GNELane(GNEEdge* edge, const int index);

    /// @brief Destructor
    ~GNELane();

    /// @brief get arent edge
    GNEEdge* getParentEdge() const;

    /// @brief check if current lane allow pedestrians
    bool allowPedestrians() const;

    /// @name Functions related with geometry of element
    /// @{
    /// @brief get elements shape
    const PositionVector& getLaneShape() const;

    /// @brief get rotations of the single shape parts
    const std::vector<double>& getShapeRotations() const;

    /// @brief get lengths of the single shape parts
    const std::vector<double>& getShapeLengths() const;

    /// @brief get dotted lane geometry
    const GNEGeometry::DottedGeometry& getDottedLaneGeometry() const;

    /// @brief update pre-computed geometry information
    void updateGeometry();

    /// @brief Returns position of hierarchical element in view
    Position getPositionInView() const;
    /// @}

    /// @name Functions related with move elements
    /// @{
    /// @brief get move operation for the given shapeOffset (can be nullptr)
    GNEMoveOperation* getMoveOperation(const double shapeOffset);

    /// @brief remove geometry point in the clicked position
    void removeGeometryPoint(const Position clickedPosition, GNEUndoList* undoList);
    /// @}

    /// @brief returns a vector with the incoming GNEConnections of this lane
    std::vector<GNEConnection*> getGNEIncomingConnections();

    /// @brief returns a vector with the outgoing GNEConnections of this lane
    std::vector<GNEConnection*> getGNEOutcomingConnections();

    /// @brief update IDs of incoming connections of this lane
    void updateConnectionIDs();

    /// @brief get length geometry factor
    double getLengthGeometryFactor() const;

    /// @name inherited from GUIGlObject
    /// @{
    // @brief Returns the name of the parent object (if any)
    // @return This object's parent id
    std::string getParentName() const;

    /**@brief Returns an own popup-menu
     *
     * @param[in] app The application needed to build the popup-menu
     * @param[in] parent The parent window needed to build the popup-menu
     * @return The built popup-menu
     * @see GUIGlObject::getPopUpMenu
     */
    GUIGLObjectPopupMenu* getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent);

    /// @brief multiplexes message to two targets
    long onDefault(FXObject*, FXSelector, void*);

    /// @brief update centering boundary (implies change in RTREE)
    void updateCenteringBoundary(const bool updateGrid);

    /**@brief Draws the object
     * @param[in] s The settings for the current view (may influence drawing)
     * @see GUIGlObject::drawGL
     */
    void drawGL(const GUIVisualizationSettings& s) const;
    /// @}

    /// @brief returns the index of the lane
    int getIndex() const;

    /// @brief returns the current speed of lane
    double getSpeed() const;

    /* @brief method for setting the index of the lane
     * @param[in] index The new index of lane
     */
    void setIndex(int index);

    /** @brief returns the parameteric length of the lane
     *  @note is the same as their Edge parent, and cannot be never nullptr
     */
    double getLaneParametricLength() const;

    /// @brief returns the length of the lane's shape
    double getLaneShapeLength() const;

    /// @brief check if this lane is restricted
    bool isRestricted(SUMOVehicleClass vclass) const;

    /// @brief get Lane2laneConnection struct
    const GNEGeometry::Lane2laneConnection& getLane2laneConnections() const;

    /// @name inherited from GNEAttributeCarrier
    /// @{
    /* @brief method for getting the Attribute of an XML key
     * @param[in] key The attribute key
     * @return string with the value associated to key
     */
    std::string getAttribute(SumoXMLAttr key) const;
    std::string getAttributeForSelection(SumoXMLAttr key) const;

    /* @brief method for setting the attribute and letting the object perform additional changes
     * @param[in] key The attribute key
     * @param[in] value The new value
     * @param[in] undoList The undoList on which to register changes
     */
    void setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList);

    /* @brief method for checking if the key and their correspond attribute are valids
     * @param[in] key The attribute key
     * @param[in] value The value asociated to key key
     * @return true if the value is valid, false in other case
     */
    bool isValid(SumoXMLAttr key, const std::string& value);

    /* @brief method for check if the value for certain attribute is set
     * @param[in] key The attribute key
     */
    bool isAttributeEnabled(SumoXMLAttr key) const;
    /// @}

    /// @brief get parameters map
    const std::map<std::string, std::string>& getACParametersMap() const;

    /// @brief add path additional element (used by GNEPathElement)
    void addPathAdditionalElement(GNEAdditional* additionalElement);

    /// @brief remove path additional element (used by GNEPathElement)
    void removePathAdditionalElement(GNEAdditional* additionalElement);

    /// @brief add path demand element (used by GNEPathElement)
    void addPathDemandElement(GNEDemandElement* demandElement);

    /// @brief remove path demand element (used by GNEPathElement)
    void removePathDemandElement(GNEDemandElement* demandElement);

    /// @brief add path demand element (used by GNEPathElement)
    void addPathGenericData(GNEGenericData* genericData);

    /// @brief remove path demand element (used by GNEPathElement)
    void removePathGenericData(GNEGenericData* genericData);

    /// @brief invalidate path element childs
    void invalidatePathElements();

    /* @brief method for setting the special color of the lane
     * @param[in] color Pointer to new special color
     */
    void setSpecialColor(const RGBColor* Color2, double colorValue = std::numeric_limits<double>::max());

    /// @brief return value for lane coloring according to the given scheme
    double getColorValue(const GUIVisualizationSettings& s, int activeScheme) const;

    /// @brief whether to draw this lane as a railway
    bool drawAsRailway(const GUIVisualizationSettings& s) const;

protected:
    /// @brief FOX needs this
    GNELane();

private:
    /// @brief parent edge (GNELanes cannot use hierarchical structures)
    GNEEdge* myParentEdge;

    /// @brief The index of this lane
    int myIndex;

    /// @brief lane geometry
    GNEGeometry::Geometry myLaneGeometry;

    /// @brief dotted lane geometry
    GNEGeometry::DottedGeometry myDottedLaneGeometry;

    /// @name computed only once (for performance) in updateGeometry()
    /// @{

    /// @brief Position of textures of restricted lanes
    std::vector<Position> myLaneRestrictedTexturePositions;

    /// @brief Rotations of textures of restricted lanes
    std::vector<double> myLaneRestrictedTextureRotations;
    /// @}

    /// @brief optional special color
    const RGBColor* mySpecialColor;

    /// @brief optional value that corresponds to which the special color corresponds
    double mySpecialColorValue;

    /// @brief The color of the shape parts (cached)
    mutable std::vector<RGBColor> myShapeColors;

    /// @brief lane2lane connections
    GNEGeometry::Lane2laneConnection myLane2laneConnections;

    /// @brief map with references to path additional elements
    std::map<SumoXMLTag, std::vector<GNEAdditional*> > myPathAdditionalElements;

    /// @brief map with references to path demand elements
    std::map<SumoXMLTag, std::vector<GNEDemandElement*> > myPathDemandElements;

    /// @brief map with references to path generic data elements
    std::map<SumoXMLTag, std::vector<GNEGenericData*> > myPathGenericDatas;

    /// @brief set attribute after validation
    void setAttribute(SumoXMLAttr key, const std::string& value);

    /// @brief set move shape
    void setMoveShape(const GNEMoveResult& moveResult);

    /// @brief commit move shape
    void commitMoveShape(const GNEMoveResult& moveResult, GNEUndoList* undoList);

    /// @brief draw children
    void drawChildren(const GUIVisualizationSettings& s) const;

    /// @brief path additional elements
    void drawPathAdditionalElements(const GUIVisualizationSettings& s) const;

    /// @brief path demand elements
    void drawPathDemandElements(const GUIVisualizationSettings& s) const;

    /// @brief path generic data elements
    void drawPathGenericDataElements(const GUIVisualizationSettings& s) const;

    /// @brief draw lane markings
    void drawMarkings(const GUIVisualizationSettings& s, const double exaggeration, const bool drawRailway) const;

    /// @brief draw link Number
    void drawLinkNo(const GUIVisualizationSettings& s) const;

    /// @brief draw TLS Link Number
    void drawTLSLinkNo(const GUIVisualizationSettings& s) const;

    /// @brief draw link rules
    void drawLinkRules(const GUIVisualizationSettings& s) const;

    /// @brief draw arrows
    void drawArrows(const GUIVisualizationSettings& s) const;

    /// @brief draw lane to lane connections
    void drawLane2LaneConnections() const;

    /// @brief sets the color according to the current scheme index and some lane function
    bool setFunctionalColor(int activeScheme, RGBColor& col) const;

    /// @brief sets multiple colors according to the current scheme index and some lane function
    bool setMultiColor(const GUIVisualizationSettings& s, const GUIColorer& c, RGBColor& col) const;

    /// @brief whether to draw this lane as a waterways
    bool drawAsWaterway(const GUIVisualizationSettings& s) const;

    /// @brief direction indicators for lanes
    void drawDirectionIndicators(const GUIVisualizationSettings& s, double exaggeration, const bool drawAsRailway, const bool spreadSuperposed) const;

    /// @brief draw lane as railway
    void drawLaneAsRailway(const GUIVisualizationSettings& s, const LaneDrawingConstants& laneDrawingConstants) const;

    /// @brief draw lane textures
    void drawTextures(const GUIVisualizationSettings& s, const LaneDrawingConstants& laneDrawingConstants) const;

    /// @brief draw start and end shape points
    void drawStartEndShapePoints(const GUIVisualizationSettings& s) const;

    /// @brief set color according to edit mode and visualisation settings
    RGBColor setLaneColor(const GUIVisualizationSettings& s) const;

    /// @brief build edge operations contextual menu
    void buildEdgeOperations(GUISUMOAbstractView& parent, GUIGLObjectPopupMenu* ret);

    /// @brief build lane operations contextual menu
    void buildLaneOperations(GUISUMOAbstractView& parent, GUIGLObjectPopupMenu* ret);

    /// @brief build rechable operations contextual menu
    void buildRechableOperations(GUISUMOAbstractView& parent, GUIGLObjectPopupMenu* ret);

    /// @brief Invalidated copy constructor.
    GNELane(const GNELane&) = delete;

    /// @brief Invalidated assignment operator.
    GNELane& operator=(const GNELane&) = delete;
};
