// Gmsh - Copyright (C) 1997-2018 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include "GmshConfig.h"
#include "GmshMessage.h"
#include "GModel.h"
#include "GModelIO_OCC.h"
#include "OCCVertex.h"
#include "OCCEdge.h"
#include "OCCFace.h"
#include "OCCRegion.h"

#if defined(HAVE_OCC)

#include <Bnd_Box.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Builder.hxx>
#include <BRepBndLib.hxx>

OCCRegion::OCCRegion(GModel *m, TopoDS_Solid _s, int num)
  : GRegion(m, num), s(_s)
{
  setup();
  if(model()->getOCCInternals())
    model()->getOCCInternals()->bind(s, num);
}

OCCRegion::~OCCRegion()
{
  if(model()->getOCCInternals() && !model()->isBeingDestroyed())
    model()->getOCCInternals()->unbind(s, tag()); // potentially slow
}

void OCCRegion::setup()
{
  l_faces.clear();
  TopExp_Explorer exp2, exp3;
  for(exp2.Init(s, TopAbs_SHELL); exp2.More(); exp2.Next()){
    TopoDS_Shape shell = exp2.Current();
    Msg::Debug("OCC Region %d - New Shell",tag());
    for(exp3.Init(shell, TopAbs_FACE); exp3.More(); exp3.Next()){
      TopoDS_Face face = TopoDS::Face(exp3.Current());
      GFace *f = 0;
      if(model()->getOCCInternals())
        f = model()->getOCCInternals()->getFaceForOCCShape(model(), face);
      if(!f){
        Msg::Error("Unknown face in region %d", tag());
      }
      else if (face.Orientation() == TopAbs_INTERNAL){
        Msg::Debug("Adding embedded face %d in region %d", f->tag(), tag());
        embedded_faces.push_back(f);
      }
      else{
        l_faces.push_back(f);
        f->addRegion(this);
      }
    }
  }

  for (exp3.Init(s, TopAbs_EDGE); exp3.More(); exp3.Next()){
    TopoDS_Edge edge = TopoDS::Edge(exp3.Current());
    GEdge *e = 0;
    if(model()->getOCCInternals())
      e = model()->getOCCInternals()->getEdgeForOCCShape(model(), edge);
    if(!e){
      Msg::Error("Unknown edge in region %d", tag());
    }
    else if (edge.Orientation() == TopAbs_INTERNAL){
      Msg::Debug("Adding embedded edge %d in region %d", e->tag(), tag());
      embedded_edges.push_back(e);
      //OCCEdge *occe = (OCCEdge*)e;
      //occe->setTrimmed(this);
    }
  }

  for (exp3.Init(s, TopAbs_VERTEX); exp3.More(); exp3.Next()){
    TopoDS_Vertex vertex = TopoDS::Vertex(exp3.Current());
    GVertex *v = 0;
    if(model()->getOCCInternals())
      v = model()->getOCCInternals()->getVertexForOCCShape(model(), vertex);
    if (!v){
      Msg::Error("Unknown vertex in region %d", tag());
    }
    else if (vertex.Orientation() == TopAbs_INTERNAL){
      Msg::Debug("Adding embedded vertex %d in region %d", v->tag(), tag());
      embedded_vertices.push_back(v);
    }
  }

  Msg::Debug("OCC Region %d with %d faces", tag(), l_faces.size());
}

SBoundingBox3d OCCRegion::bounds(bool fast) const
{
  Bnd_Box b;
  try{
    BRepBndLib::Add(s, b);
  }
  catch(Standard_Failure &err){
    Msg::Error("OpenCASCADE exception %s", err.GetMessageString());
    return SBoundingBox3d();
  }
  double xmin, ymin, zmin, xmax, ymax, zmax;
  b.Get(xmin, ymin, zmin, xmax, ymax, zmax);
  SBoundingBox3d bbox(xmin, ymin, zmin, xmax, ymax, zmax);
  return bbox;
}

GEntity::GeomType OCCRegion::geomType() const
{
  return Volume;
}

#endif
