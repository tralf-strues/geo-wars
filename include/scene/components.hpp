/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file components.hpp
 * @date 2022-05-22
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 Nikita Mochalov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "math/mat3.hpp"
#include "renderer/camera.hpp"
#include "renderer/draw_primitives.hpp"
#include "scene/script.hpp"

namespace gwars {

struct TransformComponent
{
    Vec2f translation;
    float rotation;
    Vec2f scale;

    TransformComponent(Vec2f translation = Vec2f(0, 0), float rotation = 0, Vec2f scale = Vec2f(1, 1))
        : translation(translation), rotation(rotation), scale(scale)
    {
    }

    Mat3f calculateMatrix() const
    {
        return calculateTranslationMatrix() * calculateRotationMatrix() * calculateScaleMatrix();
    }

    Mat3f calculateInverseMatrix() const
    {
        assert(scale.x > 0);
        assert(scale.y > 0);

        return gwars::scaleMatrix(Vec2f(1 / scale.x, 1 / scale.y)) * gwars::rotationMatrix(-rotation)
               * gwars::translationMatrix(-translation);
    }

    Mat3f calculateTranslationMatrix() const { return gwars::translationMatrix(translation); }
    Mat3f calculateRotationMatrix() const { return gwars::rotationMatrix(rotation); }
    Mat3f calculateScaleMatrix() const { return gwars::scaleMatrix(scale); }
};

struct CameraComponent
{
    OrthographicCameraSpecs cameraSpecs;
    bool                    isMain;

    CameraComponent() = default;
    CameraComponent(const OrthographicCameraSpecs& cameraSpecs, bool isMain = false)
        : cameraSpecs(cameraSpecs), isMain(isMain)
    {
    }
};

struct PolygonComponent
{
    Polygon polygon;

    PolygonComponent() = default;
    PolygonComponent(const Polygon& polygon) : polygon(polygon) {}
};

struct ScriptComponent
{
    INativeScript* nativeScript;

    ScriptComponent(INativeScript* nativeScript = nullptr) : nativeScript(nativeScript) {}

    ~ScriptComponent() { delete nativeScript; }
};

struct PhysicsComponent
{
    Vec2f velocity{0, 0};
    Vec2f force{0, 0};
    float mass{1};

    PhysicsComponent(const Vec2f& velocity = Vec2f(0, 0)) : velocity(velocity) {}
};

struct BoundingSphereComponent
{
    Vec2f msTranslation{0, 0};
    float msRadius;

    Vec2f wsTranslation{0, 0};
    float wsRadius{0};

    BoundingSphereComponent(float msRadius = 1, Vec2f msTranslation = Vec2f(0, 0))
        : msTranslation(msTranslation), msRadius(msRadius)
    {
    }
};

} // namespace gwars