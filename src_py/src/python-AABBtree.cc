/**
 * PYTHON Wrapper for Clothoids
 * 
 * License MIT - See LICENSE file
 * 2019 Matteo Ragni, Claudio Kerov Ghiglianovich,
 *      Enrico Bertolazzi, Marco Frego
 */

#include "python-AABBtree.hh"
#include "pybind11/stl.h"
#include <memory>
#include <string>
#include <sstream>

using namespace pybind11::literals;

namespace G2lib {
  namespace python {
    void wrap_AABBtree(py::module & m) {
      using PyPtrBBox = std::shared_ptr<BBox>;
      using PyVecPtrBBox = std::vector<PyPtrBBox>;
      using PyPairBBox = std::pair<PyPtrBBox, PyPtrBBox>;
      using PyVecPairBBox = std::vector<PyPairBBox>;

      const auto bbox_to_dict = [](const BBox & self) {
        return py::dict(
          "xmin"_a=self.Xmin(),
          "ymin"_a=self.Ymin(),
          "xmax"_a=self.Xmax(),
          "ymax"_a=self.Ymax(),
          "id"_a=self.Id(),
          "ipos"_a=self.Ipos()
        );
      };

      const auto bbox_from_dict = [](const py::dict & state) {
        const std::vector<std::string> keys({"xmin", "ymin", "xmax", "ymax", "id", "ipos"});
        for (const auto & key: keys) {
          if (!state.contains(key)) { 
            char error[128];
            std::snprintf(error, 128, "Missing `%s` in state for BBox", key.c_str());
            throw std::runtime_error(error); 
          }

          if (state[py::cast(key)].is_none()) {
            char error[128];
            std::snprintf(error, 128, "Missing `%s` in state for BBox (is None)", key.c_str());
            throw std::runtime_error(error); 
          }
        }
        
        real_type xmin = py::cast<real_type>(state["xmin"]);
        real_type ymin = py::cast<real_type>(state["ymin"]);
        real_type xmax = py::cast<real_type>(state["xmax"]);
        real_type ymax = py::cast<real_type>(state["ymax"]);
        int_type id = py::cast<int_type>(state["id"]);
        int_type ipos = py::cast<int_type>(state["ipos"]);
        return std::make_shared<BBox>(xmin, ymin, xmax, ymax, id, ipos);
      };

      py::class_<BBox, PyPtrBBox>(m, "BBox", 
      R"S(
        Class to manipulate Bounding Boxes

        Construct a bounding box with additional information
        
        There are two possible constructors. The first one, from coordinates:

        :param xmin: x-minimimum box coordinate
        :param ymin: y-minimimum box coordinate
        :param xmax: x-maximum box coordinate
        :param ymax: y-maximum box coordinate
        :param id:   identifier of the box
        :param ipos: ranking position of the box

        and the second one, from other bounding boxes:

        :param bboxes: list of bounding box
        :param id:     identifier of the box
        :param ipos:   ranking position of the box

        There is a third one, that follows some patterns exposed in python

        :param extrema: a tuple of float pairs representing the minimum and 
                        maximum point
        :param id:      identifier of the box
        :param ipos:    ranking poisition of the box
      )S")
      
      .def(py::init<real_type, real_type, real_type, real_type, int_type, int_type>(),
        py::arg("xmin"), py::arg("ymin"), py::arg("xmax"), py::arg("ymax"), py::arg("id"), py::arg("ipos"))
      
      .def(py::init([](PyVecPtrBBox bboxes, int_type id, int_type ipos) {
        std::vector<BBox::PtrBBox> _bboxes;
        for (auto & el: bboxes) {
          _bboxes.push_back(static_cast<BBox::PtrBBox>(el));
        }
        return std::make_shared<BBox>(_bboxes, id, ipos);
      }), py::arg("bboxes"), py::arg("id"), py::arg("ipos"))
      
      .def(py::init([](std::tuple<std::tuple<real_type, real_type>, std::tuple<real_type, real_type>> extrema, int_type id, int_type ipos) {
        const real_type x_min = std::get<0>(std::get<0>(extrema));
        const real_type y_min = std::get<0>(std::get<1>(extrema));
        const real_type x_max = std::get<1>(std::get<0>(extrema));
        const real_type y_max = std::get<1>(std::get<1>(extrema));
        return std::make_shared<BBox>(x_min, y_min, x_max, y_max, id, ipos);
      }), py::arg("extrema"), py::arg("id"), py::arg("ipos"))

      .def(py::pickle(bbox_to_dict, bbox_from_dict))

      .def("to_dict", bbox_to_dict)

      .def_static("from_dict", bbox_from_dict, py::arg("state"),
      R"S(
        Creates an object from a dictionary, compatible with the 
        one created with ``to_dict``.

        :param state: the input dictionary
        :return: a new BBox object
        :raises RuntimeError: if the state object is not compatible
      )S")

      .def("Xmin", [](BBox const & self) { return self.x_min(); },
      R"S(
        Minimum **x** coordinate of the bounding box

        .. warning:: Depracated: use ``x_min``

        :return: minimum **x** coordinate of the bounding box
        :rtype: float
      )S")
      
      .def("Ymin", [](BBox const & self) { return self.y_min(); }, 
      R"S(
        Minimum **y** coordinate of the bounding box

        .. warning:: Depracated: use ``y_min``

        :return: minimum **y** coordinate of the bounding box
        :rtype: float
      )S")
      
      .def("Xmax", [](BBox const & self) { return self.x_max(); },
      R"S(
        Maximum **x** coordinate of the bounding box

        .. warning:: Depracated: use ``x_max``
        
        :return: maximum **x** coordinate of the bounding box
        :rtype: float
      )S")
      
      .def("Ymax", [](BBox const & self) { return self.y_max(); },
      R"S(
        Maximum **y** coordinate of the bounding box

        .. warning:: Depracated: use ``y_max``

        :return: maximum **y** coordinate of the bounding box
        :rtype: float
      )S")

      .def("x_min", [](BBox const & self) { return self.x_min(); },
      R"S(
        Minimum **x** coordinate of the bounding box

        :return: minimum **x** coordinate of the bounding box
        :rtype: float
      )S")
      
      .def("y_min", [](BBox const & self) { return self.y_min(); }, 
      R"S(
        Minimum **y** coordinate of the bounding box

        :return: minimum **y** coordinate of the bounding box
        :rtype: float
      )S")
      
      .def("x_max", [](BBox const & self) { return self.x_max(); },
      R"S(
        Maximum **x** coordinate of the bounding box

        :return: maximum **x** coordinate of the bounding box
        :rtype: float
      )S")
      
      .def("y_max", [](BBox const & self) { return self.y_max(); },
      R"S(
        Maximum **y** coordinate of the bounding box

        :return: maximum **y** coordinate of the bounding box
        :rtype: float
      )S")

      .def("x_min", [](BBox & self, real_type v) -> real_type { self.x_min() = v; return self.x_min(); },
      R"S(
        Set minimum **x** coordinate of the bounding box

        :param v: value to set
        :return: minimum **x** coordinate of the bounding box
        :rtype: float
      )S", py::arg("v"))
      
      .def("y_min", [](BBox & self, real_type v) -> real_type { self.y_min() = v; return self.y_min(); }, 
      R"S(
        Set minimum **y** coordinate of the bounding box

        :param v: value to set
        :return: minimum **y** coordinate of the bounding box
        :rtype: float
      )S", py::arg("v"))
      
      .def("x_max", [](BBox & self, real_type v) -> real_type { self.x_max() = v; return self.x_max(); },
      R"S(
        Set maximum **x** coordinate of the bounding box

        :param v: value to set
        :return: maximum **x** coordinate of the bounding box
        :rtype: float
      )S", py::arg("v"))
      
      .def("y_max", [](BBox & self, real_type v) -> real_type { self.y_max() = v; return self.y_max(); },
      R"S(
        Set maximum **y** coordinate of the bounding box

        :param v: value to set
        :return: maximum **y** coordinate of the bounding box
        :rtype: float
      )S", py::arg("v"))
      
      .def("Id", &BBox::Id,
      R"S(
        Returns the bounding box identifier

        :return: returns the bounding box identifier
        :rtype: int
      )S")
      
      .def("Ipos", &BBox::Ipos, 
      R"S(
        Returns the bounding box position

        :return: returns the bounding box position
        :rtype: int
      )S")
      
      .def("collision", &BBox::collision, py::arg("box"),
      R"S(
        Detects if two bounding boxes collide

        :param box: the second box
        :return: a boolean on the collision 
        :rtype: bool
      )S")

      .def("join", [](PyPtrBBox self, PyVecPtrBBox bboxes) {
        std::vector<BBox::PtrBBox> _bboxes;
        for (auto & el: bboxes) {
          _bboxes.push_back(static_cast<BBox::PtrBBox>(el));
        }
        self->join(_bboxes);
      }, py::arg("bboxes"),
      R"S(
        Rebuild the current bounding box from a list of bounding boxes

        :param bboxes: a list of bounding boxes
        :return: nothing, modifies in place
        :rtype: NoneType
      )S")
      
      .def("distance", &BBox::distance, py::arg("x"), py::arg("y"),
      R"S(
        Distance between the point **(x, y)** and the bounding box

        :param x: **x** coordinates of the point
        :param y: **y** coordinates of the point
        :return: a value with the distance of the point
        :rtype: float
      )S")
      
      .def("maxDistance", &BBox::maxDistance, py::arg("x"), py::arg("y"),
      R"S(
        Maximum distance between the point **(x, y)** and the bounding box

        :param x: **x** coordinates of the point
        :param y: **y** coordinates of the point
        :return: a value with the distance of the point
        :rtype: float
      )S")
        
      .def("__str__", [](PyPtrBBox self) {
        std::ostringstream str;
        self->print(str);
        return str.str();
      });


      py::class_<AABBtree>(m, "AABBtree", 
      R"S(
        Class to build and manage an AABB tree (Axis-Aligned Bounding Box Trees)
        
        The class provides 2-dimensional aabb-tree construction and search
        for arbitrary collections of spatial objects.
        These tree-based indexing structures are useful when seeking to implement
        efficient spatial queries, reducing the complexity of intersection tests
        between collections of objects.
      )S")
      
      .def(py::init<>())
  
      .def("clear", &AABBtree::clear, 
      R"S(
        Initialized AABBtree. Works in place.

        :return: nothing
        :rtype: NoneType
      )S")

      .def("empty", &AABBtree::empty, 
      R"S(
        Check if the AABBtree is empty

        :return: check if the AABBtree is empty
        :rtype: bool
      )S")

      .def("bbox", [](const AABBtree & self) {
        real_type x_min, y_min, x_max, y_max;
        self.bbox(x_min, y_min, x_max, y_max);
        return std::make_tuple(
          std::make_tuple(x_min, y_min), 
          std::make_tuple(x_max, y_max));
      }, 
      R"S(
        Returns the extreme points of the bounding box of the the AABB tree

        :return: etrema of the bounding box, minimum (x, y) and maximum (x, y)
        :rtype: Tuple[Tuple[float, float], Tuple[float, float]]
      )S")
      
      .def("build", [](AABBtree & self, PyVecPtrBBox bboxes) {
        std::vector<BBox::PtrBBox> _bboxes;
        for (auto & el: bboxes) {
          _bboxes.push_back(static_cast<BBox::PtrBBox>(el));
        }
        self.build(_bboxes);
      }, py::arg("bboxes"),
      R"S(
        Build an AABBtree given a list of bounding boxes. Works in place.

        :param boxes: bounding boxes
        :return: Nothing, works in place
        :rtype: NoneType
      )S")

      .def("intersect", [](AABBtree & self, AABBtree const & tree, bool swap_tree) {
        AABBtree::VecPairPtrBBox intersectionList;
        PyVecPairBBox result;
        self.intersect(tree, intersectionList, swap_tree);
        for (auto & el: intersectionList) {
          BBox::PtrBBox left = std::get<0>(el);
          BBox::PtrBBox right = std::get<1>(el);
          PyPtrBBox _left = std::make_shared<BBox>(left->Xmin(), left->Ymin(), left->Xmax(), left->Ymax(), left->Id(), left->Ipos());
          PyPtrBBox _right = std::make_shared<BBox>(right->Xmin(), right->Ymin(), right->Xmax(), right->Ymax(), right->Id(), right->Ipos());
          result.push_back(std::make_pair(_left, _right));
        }
        return result;
      }, 
      R"S(
        Compute all the intersection of AABB trees.
         
        :param tree: an AABB tree that is used to check collision
        :param swap_tree: if true exchange the tree in computation
        :return: intersection list of pair bbox that overlaps
        :rtype: List[Tuple[BBox, BBox]]
      )S")

      .def("min_distance", [](AABBtree & self, real_type x, real_type y) {
        AABBtree::VecPtrBBox candidateList;
        PyVecPtrBBox result;
        self.min_distance(x, y, candidateList);
        for (auto & el: candidateList) {
          result.push_back(std::make_shared<BBox>(el->Xmin(), el->Ymin(), el->Xmax(), el->Ymax(), el->Id(), el->Ipos()));
        }
        return result;
      }, py::arg("x"), py::arg("y"),
      R"S(
        Select all the bboxes candidate to be at minimum distance.
         
        :param x: x-coordinate of the point
        :param y: y-coordinate of the point
        :return: candidate list
        :rtype: List[BBox]
      )S")

      .def("__str__", [](const AABBtree & self) {
        std::ostringstream str;
        self.print(str, 0);
        return str.str();
      })

      .def("print", [](const AABBtree & self, int_type level = 0) {
        std::ostringstream str;
        self.print(str, 0);
        return str.str();
      }, py::arg("level") = 0,
      R"S(
        Pretty print an AABBtree to a certain depth level

        :param level: depth for exploration
        :return: a string with the pretty print status of the tree
        :rtype: str
      )S");
    }
  }
}