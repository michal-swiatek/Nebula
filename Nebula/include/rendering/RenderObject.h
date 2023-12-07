//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

namespace nebula::rendering {

    class RenderObjectVisitor;

    class RenderObject
    {
    public:
        virtual ~RenderObject() = default;

        virtual void accept(RenderObjectVisitor& visitor) const = 0;
    };

}

#endif //RENDEROBJECT_H
