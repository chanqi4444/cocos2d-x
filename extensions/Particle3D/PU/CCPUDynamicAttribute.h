/****************************************************************************
 Copyright (C) 2013 Henry van Merode. All rights reserved.
 Copyright (c) 2015 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CC_PU_PARTICLE_3D_DYNAMIC_ATTRIBUTE_H__
#define __CC_PU_PARTICLE_3D_DYNAMIC_ATTRIBUTE_H__

#include "base/CCRef.h"
#include "math/CCMath.h"
#include "CCPUSimpleSpline.h"
#include <vector>

NS_CC_BEGIN

enum PUInterpolationType
{
    IT_LINEAR,
    IT_SPLINE
};

/** Comparer used for sorting vector in ascending order
*/
struct PUControlPointSorter
{
    inline bool operator() (const Vec2& a, const Vec2& b)
    {
        return a.x < b.x;
    }
};

/** @class PUDynamicAttribute
*@brief @~english This class provides a uniform interface for retrieving the value of an attribute, while the calculation of
this value may vary. Each subclass provides its own implementation of the getValue() function and the calling
application doesn't need to know about the underlying logic. A subclass could just return a value that has
previously been set, but it can also return a value that is randomly generated by a pre-defined min/max interval.
The DynamicAttribute class is used in situations where different behaviour of a certain attribute is needed,
but where implementation of this behaviour may not be scattered or duplicated within the application that needs
it.
@~chinese 这个类提供了一个统一的接口获取一个属性的值,同时计算
这个值可能会有所不同。每个子类提供自己的实现getValue()函数的调用
应用程序不需要知道潜在的逻辑。可以返回一个值,一个子类
之前被设置,但它也可以返回一个值随机生成的一个预定义的最小/最大间隔。
DynamicAttribute类被用在不同的情况下的行为需要一个特定的属性,
但是,实施这种行为可能不是散布或复制应用程序的需求。
*/
class CC_DLL PUDynamicAttribute : public Ref
{
    public:
        enum DynamicAttributeType
        {
            DAT_FIXED,
            DAT_RANDOM,
            DAT_CURVED,
            DAT_OSCILLATE
        };

        /** Constructor
        */
        PUDynamicAttribute (void);

        /** Destructor
        */
        virtual ~PUDynamicAttribute (void);

        /**
        * @~english Virtual function that needs to be implemented by its childs.
        * @~chinese 需被子类实现的虚函数。
        * @return   @~english The Value. @~chinese 属性值。
        */
        virtual float getValue (float x = 0) = 0;

        /**
        * @~english Get the type of dynamic attribute.
        * @~chinese 获取动态属性的类型。
        * @return   @~english The Type. @~chinese 类型。
        */
        DynamicAttributeType getType (void) const;

        /**
        * @~english Set the type of dynamic attribute.
        * @~chinese 设置动态属性的类型。
        * @param type   @~english The Type. @~chinese 类型。
        */
        void setType (DynamicAttributeType type);

        /**
        * @~english Returns true if one of the attributes was changed by an external source.
        * @~chinese 是否被外部代码改变属性。
        * @return   @~english Whether attributes changed. @~chinese 属性是否改变。
        */
        bool isValueChangedExternally(void) const;

        /**
        * @~english Copy attributes to another PUDynamicAttribute.
        * @~chinese 把当前类的值拷贝到另一个PUDynamicAttribute。
        * @param dynamicAttribute   @~english Another dynamicAttribute. @~chinese 需拷贝的dynamicAttribute。
        */
        virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) = 0;

        /**
        * @~english Clone this PUDynamicAttribute.
        * @~chinese 克隆当前的PUDynamicAttribute。
        * @return   @~english A new PUDynamicAttribute. @~chinese 一个新的PUDynamicAttribute。
        */
        virtual PUDynamicAttribute* clone() = 0;

    protected:
        DynamicAttributeType _type;
        bool _valueChangedExternally;
};

/** @class PUDynamicAttributeFixed
*@brief @~english This class is an implementation of the DynamicAttribute class in its most simple form. It just returns a value
that has previously been set.Although use of a regular attribute within the class that needs it is preferred, its benefit is that it makes
use of the generic 'getValue' mechanism of a DynamicAttribute.
@~chinese 这个类是一个实现DynamicAttribute类在其最简单的形式。它只是返回一个
曾被设置过的固定值.尽管使用常规属性该类是首选,但是它更好的价值在于使
使用通用的getValue的DynamicAttribute的机制。
*/
class CC_DLL PUDynamicAttributeFixed : public PUDynamicAttribute
{
    public:
        /** Constructor
        */
        PUDynamicAttributeFixed (void);

        /** Copy constructor
        */
        PUDynamicAttributeFixed (const PUDynamicAttributeFixed& dynamicAttributeFixed);

        /** Destructor
        */
        ~PUDynamicAttributeFixed (void);

        /** override */
        virtual float getValue (float x = 0) override;

        /**
        * @~english Set the fixed value.
        * @~chinese 设置固定值。
        * @param type   @~english The fixed value. @~chinese 固定值。
        */
        virtual void setValue (float value);

        /** override */
        virtual PUDynamicAttributeFixed* clone() override;

        /** override */
        virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

    protected:
        float _value;
};

/** @class PUDynamicAttributeRandom
*@brief @~english This class generates random values within a given minimum and maximum interval.
@~chinese 该类根据设定的最大最小值返回一个之间的随机数。
*/
class CC_DLL PUDynamicAttributeRandom : public PUDynamicAttribute
{
    public:
        /** Constructor
        */
        PUDynamicAttributeRandom (void);

        /** Copy constructor
        */
        PUDynamicAttributeRandom (const PUDynamicAttributeRandom& dynamicAttributeRandom);

        /** Destructor
        */
        ~PUDynamicAttributeRandom (void);

        /** override*/
        virtual float getValue (float x = 0) override;

        /**
        * @~english Sets the minimum value.
        * @~chinese 设置最小值。
        * @param min   @~english The minimum value. @~chinese 最小值。
        */
        void setMin (float min);

        /**
        * @~english Gets the minimum value.
        * @~chinese 获取最小值。
        * @return   @~english The minimum value. @~chinese 最小值。
        */
        float getMin (void) const;

        /**
        * @~english Sets the maximum value.
        * @~chinese 设置最大值。
        * @param max   @~english The maximum value. @~chinese 最大值。
        */
        void setMax (float max);

        /**
        * @~english Sets the maximum value.
        * @~chinese 设置最大值。
        * @return   @~english The maximum value. @~chinese 最大值。
        */
        float getMax (void) const;

        /**
        * @~english Sets the minimum and maximum values.
        * @~chinese 设置最小最大值。
        * @param min   @~english The minimum value. @~chinese 最小值。
        * @param max   @~english The maximum value. @~chinese 最大值。
        */
        void setMinMax (float min, float max);

        /** override*/
        virtual PUDynamicAttributeRandom* clone() override;

        /** override*/
        virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

protected:
        float _min, _max;
};

/** @class PUDynamicAttributeCurved
*@brief @~english This is a more complex usage of the DynamicAttribute principle. This class returns a value on an curve.
After setting a number of control points, this class is able to interpolate a point on the curve that is based
on these control points. Interpolation is done in different flavours. linear?provides linear interpolation
of a value on the curve, while spline?generates a smooth curve and the returns a value that lies on that curve.
@~chinese 这是一个DynamicAttribute更复杂的实现，它返回的是一条曲线。
设置一定数量的控制点后,这个类可以根据控制点进行插值。例如线性插值和样条插值。
*/
class CC_DLL PUDynamicAttributeCurved : public PUDynamicAttribute
{
    public:
        typedef std::vector<Vec2> ControlPointList;

        /** Constructor
        */
        PUDynamicAttributeCurved (void);
        PUDynamicAttributeCurved (PUInterpolationType interpolationType);

        /** Copy constructor
        */
        PUDynamicAttributeCurved (const PUDynamicAttributeCurved& dynamicAttributeCurved);

        /** Destructor
        */
        ~PUDynamicAttributeCurved (void);

        /**
        * @~english Sets the interpolation type.
        * @~chinese 设置插值类型。
        * @param interpolationType   @~english The interpolation type. @~chinese 插值类型。
        */
        void setInterpolationType (PUInterpolationType interpolationType);

        /**
        * @~english Gets the interpolation type.
        * @~chinese 获取插值类型。
        * @return   @~english The interpolation type. @~chinese 插值类型。
        */
        PUInterpolationType getInterpolationType (void) const;

        /** override*/
        virtual float getValue (float x = 0) override;

        /**
        * @~english Add the control points.
        * @~chinese 设置控制点。
        * @param x   @~english The x value. @~chinese 控制点的x坐标值。
        * @param y   @~english The y value. @~chinese 控制点的y坐标值。
        */
        virtual void addControlPoint (float x, float y);

        /**
        * @~english Get the control points.
        * @~chinese 获取所有控制点。
        * @return   @~english The all control points. @~chinese 所有的控制点。
        */
        const ControlPointList& getControlPoints (void) const;

        /**
        * @~english Process the control points.
        * @~chinese 处理所有控制点。
        */
        void processControlPoints (void);

        /**
        * @~english Get the number of control points.
        * @~chinese 获取控制点数目。
        * @return   @~english The number of control points. @~chinese 控制点数目。
        */
        size_t getNumControlPoints(void) const;

        /**
        * @~english Remove all control points.
        * @~chinese 移除所有控制点。
        */
        void removeAllControlPoints(void);

        /** override*/
        virtual PUDynamicAttributeCurved* clone() override;

        /** override*/
        virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

    protected:

        /** Todo
        */
        float _range;

        /** Todo
        */
        PUSimpleSpline _spline;

        /** Todo
        */
        PUInterpolationType _interpolationType;

        /** Todo
        */
        ControlPointList _controlPoints;

        /** Find an iterator that forms the low (left) value of the interval where x lies in.
        */
        inline ControlPointList::iterator findNearestControlPointIterator(float x);

        /** Helper functions 
        */
        inline ControlPointList::iterator getFirstValidIterator(void);
        inline ControlPointList::iterator getLastValidIterator(void);
};

/** @class PUDynamicAttributeOscillate
*@brief @~english This class generates values based on an oscillating function (i.e. Sine)
@~chinese 这个类根据波动方程生成一个值。
*/
class CC_DLL PUDynamicAttributeOscillate : public PUDynamicAttribute
{
    public:
        enum OscillationType
        {
            OSCT_SINE,
            OSCT_SQUARE
        };

        /** Constructor
        */
        PUDynamicAttributeOscillate (void);

        /** Copy constructor
        */
        PUDynamicAttributeOscillate (const PUDynamicAttributeOscillate& dynamicAttributeOscillate);

        /** Destructor
        */
        ~PUDynamicAttributeOscillate (void);

        /** Todo
        */
        virtual float getValue (float x = 0) override;

        /**
        * @~english Get the oscillation type.
        * @~chinese 设置波动方程类型。
        * @return   @~english The oscillation type. @~chinese 波动方程类型。
        */
        OscillationType getOscillationType (void) const;

        /**
        * @~english Set the oscillation type.
        * @~chinese 设置波动方程类型。
        * @param oscillationType   @~english The oscillation type. @~chinese 波动方程类型。
        */
        void setOscillationType (OscillationType oscillationType);

        /**
        * @~english Get the frequency.
        * @~chinese 获取波动频率。
        * @return   @~english The frequency. @~chinese 波动频率。
        */
        float getFrequency (void) const;

        /**
        * @~english Set the frequency.
        * @~chinese 设置波动频率。
        * @param frequency   @~english The frequency. @~chinese 波动频率。
        */
        void setFrequency (float frequency);

        /**
        * @~english Get the phase.
        * @~chinese 获取波动相位。
        * @return   @~english The phase. @~chinese 波动相位。
        */
        float getPhase (void) const;

        /**
        * @~english Set the phase.
        * @~chinese 设置波动频率。
        * @param phase   @~english The phase. @~chinese 波动相位。
        */
        void setPhase (float phase);

        /**
        * @~english Get the Base Line.
        * @~chinese 获取波动方程基准线。
        * @return  @~english The Base Line. @~chinese 基准线。
        */
        float getBase (void) const;

        /**
        * @~english Set the Base Line.
        * @~chinese 设置波动方程基准线。
        * @param base   @~english The Base Line. @~chinese 基准线。
        */
        void setBase (float base);

        /**
        * @~english Get the amplitude.
        * @~chinese 获取波动振幅。
        * @return  @~english The amplitude. @~chinese 波动振幅。
        */
        float getAmplitude (void) const;

        /**
        * @~english Set the amplitude.
        * @~chinese 设置波动振幅。
        * @param amplitude   @~english The amplitude. @~chinese 波动振幅。
        */
        void setAmplitude (float amplitude);

        /** override*/
        virtual PUDynamicAttributeOscillate* clone() override;

        /** override*/
        virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

    protected:
        OscillationType _oscillationType;
        float  _frequency;
        float  _phase;
        float  _base;
        float  _amplitude;
};

/** @class PUDynamicAttributeHelper
*@brief @~english Helper class to do some generic calculation
@~chinese 计算用的帮助类。
*/
class PUDynamicAttributeHelper
{
    public:
        /**
        * @~english Return PUDynamicAttribute getvalue if dyn != null, otherwise, return defaultValue.
        * @~chinese 当dyn的指针不为空时返回PUDynamicAttribute的getValue函数，否则，返回defaultValue。
        * @return  @~english The Value. @~chinese 计算值。
        */
        float calculate(PUDynamicAttribute* dyn, float x, float defaultValue = 0.0f);
};
NS_CC_END

#endif
