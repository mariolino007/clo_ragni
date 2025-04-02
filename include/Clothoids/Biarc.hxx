/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @file Biarc.hxx
 * @author Matteo Ragni (info@ragni.me)
 *
 * @copyright Copyright (c) 2022 Matteo Ragni
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Based on the work of:
 * Enrico Bertolazzi http://ebertolazzi.github.io/Clothoids/
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma once
#include "BaseCurve.hxx"
#include "Circle.hxx"
#include "Line.hxx"

namespace G2lib {

  /*\
   |   ____  _
   |  | __ )(_) __ _ _ __ ___
   |  |  _ \| |/ _` | '__/ __|
   |  | |_) | | (_| | | | (__
   |  |____/|_|\__,_|_|  \___|
  \*/

  //!
  //! Compute biarc fitting by Hemite data
  //!
  //! \rst
  //!
  //!   .. image:: ../../images/biarc.jpg
  //!      :width: 80%
  //!      :align: center
  //!
  //! \endrst
  //!

  class Biarc : public BaseCurve {
    CircleArc m_C0, m_C1;

    void gfun(real_type alpha, real_type g[3]) const;

   public:
#include "BaseCurve_using.hxx"

    ~Biarc() override {}

    //!
    //! Construct and empty biarc
    //!
    Biarc() : BaseCurve(G2LIB_BIARC) {}

    //!
    //! Make a copy of an existing biarc
    //!
    Biarc(Biarc const & ba) : BaseCurve(G2LIB_BIARC) { copy(ba); }

    //!
    //! Construct a biarc passing from the points
    //! \f$ (x_0,y_0) \f$ to the point  \f$ (x_1,y_1) \f$
    //! with initial angle \f$ \theta_0 \f$ and final angle \f$ \theta_1 \f$
    //!
    //! \param[in] x0      \f$ x_0 \f$
    //! \param[in] y0      \f$ y_0 \f$
    //! \param[in] theta0  \f$ \theta_0 \f$
    //! \param[in] x1      \f$ x_1 \f$
    //! \param[in] y1      \f$ y_1 \f$
    //! \param[in] theta1  \f$ \theta_1 \f$
    //!
    explicit Biarc(real_type x0, real_type y0, real_type theta0, real_type x1, real_type y1, real_type theta1);

    explicit Biarc(BaseCurve const & C);

    //!
    //! Make a copy of an existing biarc.
    //!
    void copy(Biarc const & c) {
      m_C0.copy(c.m_C0);
      m_C1.copy(c.m_C1);
    }

    //!
    //! Make a copy of an existing biarc.
    //!
    Biarc const & operator=(Biarc const & ba) {
      copy(ba);
      return *this;
    }

    //!
    //! Return the first circle arc of the biarc.
    //!
    CircleArc const & C0() const { return m_C0; }

    //!
    //! Return the first circle arc of the biarc.
    //!
    CircleArc const & C1() const { return m_C1; }

    //!
    //! Construct a biarc passing from the points
    //! \f$ (x_0,y_0) \f$ to the point  \f$ (x_1,y_1) \f$
    //! with initial angle \f$ \theta_0 \f$ and final angle \f$ \theta_1 \f$
    //!
    //! \param[in] x0      \f$ x_0 \f$
    //! \param[in] y0      \f$ y_0 \f$
    //! \param[in] theta0  \f$ \theta_0 \f$
    //! \param[in] x1      \f$ x_1 \f$
    //! \param[in] y1      \f$ y_1 \f$
    //! \param[in] theta1  \f$ \theta_1 \f$
    //! \return false if biarc cannot be computed
    //!
    bool build(real_type x0, real_type y0, real_type theta0, real_type x1, real_type y1, real_type theta1);

    //!
    //! Construct a biarc by 3 point at "minimum energy"
    //!
    //! - Planar point set fairing and fitting by arc splines
    //! - Xunnian Yang and Guozhao Wang
    //! - Computer-Aided Design, vol 33, 2001
    //!
    bool build_3P(real_type x0, real_type y0, real_type x1, real_type y1, real_type x2, real_type y2);

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    void bbox(real_type & xmin, real_type & ymin, real_type & xmax, real_type & ymax) const override;

    void bbox_ISO(
        real_type offs, real_type & xmin, real_type & ymin, real_type & xmax, real_type & ymax) const override;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    real_type length() const override { return m_C0.length() + m_C1.length(); }

    real_type length_ISO(real_type offs) const override { return m_C0.length_ISO(offs) + m_C1.length_ISO(offs); }

    real_type theta_begin() const override { return m_C0.theta_begin(); }
    real_type theta_end() const override { return m_C1.theta_end(); }
    real_type kappa_begin() const override { return m_C0.kappa_begin(); }
    real_type kappa_end() const override { return m_C1.kappa_end(); }
    real_type x_begin() const override { return m_C0.x_begin(); }
    real_type x_end() const override { return m_C1.x_end(); }
    real_type y_begin() const override { return m_C0.y_begin(); }
    real_type y_end() const override { return m_C1.y_end(); }
    real_type tx_Begin() const override { return m_C0.tx_Begin(); }
    real_type tx_End() const override { return m_C1.tx_End(); }
    real_type ty_Begin() const override { return m_C0.ty_Begin(); }
    real_type ty_End() const override { return m_C1.ty_End(); }
    real_type nx_Begin_ISO() const override { return m_C0.nx_Begin_ISO(); }
    real_type nx_End_ISO() const override { return m_C1.nx_End_ISO(); }
    real_type ny_Begin_ISO() const override { return m_C0.ny_Begin_ISO(); }
    real_type ny_End_ISO() const override { return m_C1.ny_End_ISO(); }

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    real_type theta(real_type) const override;
    real_type theta_D(real_type) const override;
    real_type theta_DD(real_type) const override { return 0; }
    real_type theta_DDD(real_type) const override { return 0; }

    void evaluate(real_type s, real_type & th, real_type & k, real_type & x, real_type & y) const override;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    real_type X(real_type s) const override;
    real_type Y(real_type s) const override;

    real_type X_D(real_type) const override;
    real_type Y_D(real_type) const override;

    real_type X_DD(real_type) const override;
    real_type Y_DD(real_type) const override;

    real_type X_DDD(real_type) const override;
    real_type Y_DDD(real_type) const override;

    real_type X_ISO(real_type s, real_type offs) const override;
    real_type Y_ISO(real_type s, real_type offs) const override;

    real_type X_ISO_D(real_type, real_type offs) const override;
    real_type Y_ISO_D(real_type, real_type offs) const override;

    real_type X_ISO_DD(real_type, real_type offs) const override;
    real_type Y_ISO_DD(real_type, real_type offs) const override;

    real_type X_ISO_DDD(real_type, real_type offs) const override;
    real_type Y_ISO_DDD(real_type, real_type offs) const override;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    void eval(real_type s, real_type & x, real_type & y) const override;

    void eval_D(real_type s, real_type & x_D, real_type & y_D) const override;

    void eval_DD(real_type s, real_type & x_DD, real_type & y_DD) const override;

    void eval_DDD(real_type s, real_type & x_DDD, real_type & y_DDD) const override;

    void eval_ISO(real_type s, real_type offs, real_type & x, real_type & y) const override;

    void eval_ISO_D(real_type s, real_type offs, real_type & x_D, real_type & y_D) const override;

    void eval_ISO_DD(real_type s, real_type offs, real_type & x_DD, real_type & y_DD) const override;

    void eval_ISO_DDD(real_type s, real_type offs, real_type & x_DDD, real_type & y_DDD) const override;

    /*\
     |  _____                   _   _   _
     | |_   _|   __ _ _ __   __| | | \ | |
     |   | |    / _` | '_ \ / _` | |  \| |
     |   | |   | (_| | | | | (_| | | |\  |
     |   |_|    \__,_|_| |_|\__,_| |_| \_|
    \*/

    real_type tx(real_type s) const override;
    real_type tx_D(real_type s) const override;
    real_type tx_DD(real_type s) const override;
    real_type tx_DDD(real_type s) const override;
    real_type ty(real_type s) const override;
    real_type ty_D(real_type s) const override;
    real_type ty_DD(real_type s) const override;
    real_type ty_DDD(real_type s) const override;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    void tg(real_type s, real_type & tx, real_type & ty) const override;

    void tg_D(real_type s, real_type & tx_D, real_type & ty_D) const override;

    void tg_DD(real_type s, real_type & tx_DD, real_type & ty_DD) const override;

    void tg_DDD(real_type s, real_type & tx_DDD, real_type & ty_DDD) const override;

    /*\
     |  _                        __
     | | |_ _ __ __ _ _ __  ___ / _| ___  _ __ _ __ ___
     | | __| '__/ _` | '_ \/ __| |_ / _ \| '__| '_ ` _ \
     | | |_| | | (_| | | | \__ \  _| (_) | |  | | | | | |
     |  \__|_|  \__,_|_| |_|___/_|  \___/|_|  |_| |_| |_|
    \*/

    void translate(real_type tx, real_type ty) override {
      m_C0.translate(tx, ty);
      m_C1.translate(tx, ty);
    }

    void rotate(real_type angle, real_type cx, real_type cy) override {
      m_C0.rotate(angle, cx, cy);
      m_C1.rotate(angle, cx, cy);
    }

    void reverse() override;

    void change_origin(real_type newx0, real_type newy0) override;

    void trim(real_type s_begin, real_type s_end) override;

    void scale(real_type s) override;

    /*\
     |        _                     _   ____       _       _
     |    ___| | ___  ___  ___  ___| |_|  _ \ ___ (_)_ __ | |_
     |   / __| |/ _ \/ __|/ _ \/ __| __| |_) / _ \| | '_ \| __|
     |  | (__| | (_) \__ \  __/\__ \ |_|  __/ (_) | | | | | |_
     |   \___|_|\___/|___/\___||___/\__|_|   \___/|_|_| |_|\__|
    \*/

    int_type closest_point_ISO(
        real_type qx, real_type qy, real_type & x, real_type & y, real_type & s, real_type & t, real_type & dst)
        const override;

    int_type closest_point_ISO(
        real_type   qx,
        real_type   qy,
        real_type   offs,
        real_type & x,
        real_type & y,
        real_type & s,
        real_type & t,
        real_type & dst) const override;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    //!
    //! Return the x-coordinate of the juction point of the biarc.
    //!
    real_type x_middle() const { return m_C1.x_begin(); }

    //!
    //! Return the y-coordinate of the juction point of the biarc.
    //!
    real_type y_middle() const { return m_C1.y_begin(); }

    //!
    //! Return the angle of the juction point of the biarc.
    //!
    real_type theta_middle() const { return m_C1.theta_begin(); }

    //!
    //! Return the curvature of the first arc of the biarc.
    //!
    real_type kappa0() const { return m_C0.curvature(); }

    //!
    //! Intial angle of the biarc.
    //!
    real_type length0() const { return m_C0.length(); }

    //!
    //! Return the curvature of the first arc of the biarc.
    //!
    real_type kappa1() const { return m_C1.curvature(); }

    //!
    //! Final angle of the biarc.
    //!
    real_type length1() const { return m_C1.length(); }

    //!
    //! Change of the angle of the biarc (\f$ \theta_1-\theta_0 \f$).
    //!
    real_type delta_theta() const { return m_C0.delta_theta() + m_C1.delta_theta(); }

    void bbTriangles(
        std::vector<Triangle2D> & tvec,
        real_type                 max_angle = Utils::m_pi / 18,
        real_type                 max_size  = 1e100,
        int_type                  icurve    = 0) const override {
      m_C0.bbTriangles(tvec, max_angle, max_size, icurve);
      m_C1.bbTriangles(tvec, max_angle, max_size, icurve);
    }

    void bbTriangles_ISO(
        real_type                 offs,
        std::vector<Triangle2D> & tvec,
        real_type                 max_angle = Utils::m_pi / 18,
        real_type                 max_size  = 1e100,
        int_type                  icurve    = 0) const override {
      m_C0.bbTriangles_ISO(offs, tvec, max_angle, max_size, icurve);
      m_C1.bbTriangles_ISO(offs, tvec, max_angle, max_size, icurve);
    }

    void bbTriangles_SAE(
        real_type                 offs,
        std::vector<Triangle2D> & tvec,
        real_type                 max_angle = Utils::m_pi / 18,
        real_type                 max_size  = 1e100,
        int_type                  icurve    = 0) const override {
      m_C0.bbTriangles_SAE(offs, tvec, max_angle, max_size, icurve);
      m_C1.bbTriangles_SAE(offs, tvec, max_angle, max_size, icurve);
    }

    /*\
     |             _ _ _     _
     |    ___ ___ | | (_)___(_) ___  _ __
     |   / __/ _ \| | | / __| |/ _ \| '_ \
     |  | (_| (_) | | | \__ \ | (_) | | | |
     |   \___\___/|_|_|_|___/_|\___/|_| |_|
    \*/

    //!
    //! Detect a collision with another biarc.
    //!
    bool collision(Biarc const & B) const {
      return m_C0.collision(B.m_C0) || m_C0.collision(B.m_C1) || m_C1.collision(B.m_C0) || m_C1.collision(B.m_C1);
    }

    //!
    //! Detect a collision with another biarc with offset.
    //!
    //! \param[in] offs   offset of first biarc
    //! \param[in] B      second biarc
    //! \param[in] offs_B offset of second biarc
    //!
    bool collision_ISO(real_type offs, Biarc const & B, real_type offs_B) const {
      return m_C0.collision_ISO(offs, B.m_C0, offs_B) || m_C0.collision_ISO(offs, B.m_C1, offs_B) ||
             m_C1.collision_ISO(offs, B.m_C0, offs_B) || m_C1.collision_ISO(offs, B.m_C1, offs_B);
    }

    /*\
     |   _       _                          _
     |  (_)_ __ | |_ ___ _ __ ___  ___  ___| |_
     |  | | '_ \| __/ _ \ '__/ __|/ _ \/ __| __|
     |  | | | | | ||  __/ |  \__ \  __/ (__| |_
     |  |_|_| |_|\__\___|_|  |___/\___|\___|\__|
    \*/

    //!
    //! Intersect a biarc with another biarc.
    //!
    //! \param[in]  B           second biarc
    //! \param[out] ilist       list of the intersection (as parameter on the curves)
    //! \param[in]  swap_s_vals if true store `(s2,s1)` instead of `(s1,s2)` for each
    //!                         intersection
    //!
    void intersect(Biarc const & B, IntersectList & ilist, bool swap_s_vals) const;

    //!
    //! Intersect a biarc with another biarc with offset (ISO).
    //!
    //! \param[in]  offs        offset of first biarc
    //! \param[in]  B           second biarc
    //! \param[in]  offs_B      offset of second biarc
    //! \param[out] ilist       list of the intersection (as parameter on the curves)
    //! \param[in]  swap_s_vals if true store `(s2,s1)` instead of `(s1,s2)` for each
    //!                         intersection
    //!
    void intersect_ISO(
        real_type offs, Biarc const & B, real_type offs_B, IntersectList & ilist, bool swap_s_vals) const;

    void info(ostream_type & stream) const override { stream << "BiArc\n" << *this << '\n'; }

    //!
    //! Pretty print of the biarc.
    //!
    friend ostream_type & operator<<(ostream_type & stream, Biarc const & bi);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //!
  //! Given a list of points \f$ (x_i,y_i) \f$
  //! build a guess of angles for a spline of biarc.
  //!
  //! \param[in]  n     number of points
  //! \param[in]  x     x-coordinates
  //! \param[in]  y     y-coordinates
  //! \param[out] theta guessed angles
  //!
  bool build_guess_theta(int_type n, real_type const * x, real_type const * y, real_type * theta);

}  // namespace G2lib

///
/// eof: Biarc.hxx
///
