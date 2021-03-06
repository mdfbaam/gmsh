// Gmsh - Copyright (C) 1997-2018 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <sstream>
#include <algorithm>
#include "GModel.h"
#include "GVertex.h"
#include "GFace.h"
#include "MPoint.h"
#include "GmshMessage.h"

GVertex::GVertex(GModel *m, int tag, double ms) : GEntity(m, tag), meshSize(ms)
{
}

GVertex::~GVertex()
{
  deleteMesh();
}

void GVertex::deleteMesh(bool onlyDeleteElements)
{
  if(!onlyDeleteElements){
    for(unsigned int i = 0; i < mesh_vertices.size(); i++) delete mesh_vertices[i];
    mesh_vertices.clear();
  }
  for(unsigned int i = 0; i < points.size(); i++) delete points[i];
  points.clear();
  deleteVertexArrays();
  model()->destroyMeshCaches();
}

void GVertex::resetMeshAttributes()
{
  meshSize = MAX_LC;
}

void GVertex::setPosition(GPoint &p)
{
  Msg::Error("Cannot set position of this kind of vertex");
}

void GVertex::addEdge(GEdge *e)
{
  if(std::find(l_edges.begin(), l_edges.end(), e) == l_edges.end())
    l_edges.push_back(e);
}

void GVertex::delEdge(GEdge *e)
{
  std::list<GEdge*>::iterator it = std::find(l_edges.begin(), l_edges.end(), e);
  if(it != l_edges.end()) l_edges.erase(it);
}

SPoint2 GVertex::reparamOnFace(const GFace *gf, int) const
{
  return gf->parFromPoint(SPoint3(x(), y(), z()));
}

std::string GVertex::getAdditionalInfoString(bool multline)
{
  std::ostringstream sstream;
  sstream.precision(12);
  sstream << "Position (" << x() << ", " << y() << ", " << z() << ")";
  double lc = prescribedMeshSizeAtVertex();
  if(lc < MAX_LC){
    if(multline) sstream << "\n";
    else sstream << " ";
    sstream << "Mesh attributes: size " << lc;
  }
  return sstream.str();
}

void GVertex::writeGEO(FILE *fp, const std::string &meshSizeParameter)
{
  if(meshSizeParameter.size())
    fprintf(fp, "Point(%d) = {%.16g, %.16g, %.16g, %s};\n",
            tag(), x(), y(), z(), meshSizeParameter.c_str());
  else if(prescribedMeshSizeAtVertex() != MAX_LC)
    fprintf(fp, "Point(%d) = {%.16g, %.16g, %.16g, %.16g};\n",
            tag(), x(), y(), z(), prescribedMeshSizeAtVertex());
  else
    fprintf(fp, "Point(%d) = {%.16g, %.16g, %.16g};\n",
            tag(), x(), y(), z());
}

unsigned int GVertex::getNumMeshElements() const
{
  return points.size();
}

unsigned int GVertex::getNumMeshElementsByType(const int familyType) const
{
  if(familyType == TYPE_PNT) return points.size();

  return 0;
}

void GVertex::getNumMeshElements(unsigned *const c) const
{
  c[0] += points.size();
}

MElement *GVertex::getMeshElement(unsigned int index) const
{
  if(index < points.size())
    return points[index];
  return 0;
}

MElement *GVertex::getMeshElementByType(const int familyType, const unsigned int index) const
{
  if(familyType == TYPE_PNT) return points[index];

  return 0;
}

bool GVertex::isOnSeam(const GFace *gf) const
{
  std::list<GEdge*>::const_iterator eIter = l_edges.begin();
  for (; eIter != l_edges.end(); eIter++) {
    if ( (*eIter)->isSeam(gf) ) return true;
  }
  return false;
}

// faces that bound this entity or that this entity bounds.
std::list<GFace*> GVertex::faces() const
{
  std::list<GEdge*>::const_iterator it = l_edges.begin();
  std::set<GFace*> _f;
  for ( ; it != l_edges.end() ; ++it){
    std::list<GFace*> temp = (*it)->faces();
    _f.insert (temp.begin(), temp.end());
  }
  std::list<GFace*> ret;
  ret.insert (ret.begin(), _f.begin(), _f.end());
  return ret;
}

// regions that bound this entity or that this entity bounds.
std::list<GRegion*> GVertex::regions() const
{
  std::list<GFace*> _faces = faces();
  std::list<GFace*>::const_iterator it = _faces.begin();
  std::set<GRegion*> _r;
  for ( ; it != _faces.end() ; ++it){
    std::list<GRegion*> temp = (*it)->regions();
    _r.insert (temp.begin(), temp.end());
  }
  std::list<GRegion*> ret;
  ret.insert (ret.begin(), _r.begin(), _r.end());
  return ret;
}

void GVertex::relocateMeshVertices()
{
  for(unsigned int i = 0; i < mesh_vertices.size(); i++){
    MVertex *v = mesh_vertices[i];
    v->x() = x();
    v->y() = y();
    v->z() = z();
  }
}

void GVertex::addElement(int type, MElement *e)
{
  switch (type){
  case TYPE_PNT:
    addPoint(reinterpret_cast<MPoint*>(e));
    break;
  default:
    Msg::Error("Trying to add unsupported element in vertex");
  }
}

void GVertex::removeElement(int type, MElement *e)
{
  switch (type){
  case TYPE_PNT:
    {
      std::vector<MPoint*>::iterator it = std::find
        (points.begin(), points.end(), reinterpret_cast<MPoint*>(e));
      if(it != points.end()) points.erase(it);
    }
    break;
  default:
    Msg::Error("Trying to remove unsupported element in vertex");
  }
}

bool GVertex::reorder(const int elementType, const std::vector<int> &ordering)
{
  if(points.front()->getTypeForMSH() == elementType){
    if(ordering.size() != points.size()) return false;

    for(std::vector<int>::const_iterator it = ordering.begin();
        it != ordering.end(); ++it){
      if(*it < 0 || *it >= points.size()) return false;
    }

    std::vector<MPoint*> newPointsOrder(points.size());
    for(unsigned int i = 0; i < ordering.size(); i++){
      newPointsOrder[i] = points[ordering[i]];
    }
#if __cplusplus >= 201103L
    points = std::move(newPointsOrder);
#else
    points = newPointsOrder;
#endif

    return true;
  }

  return false;
}
