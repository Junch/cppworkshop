#include <gtest/gtest.h>

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: Breaking Dependencies: Type Erasure - A Design Analysis - Klaus Iglberger - CppCon 2021
// Web: https://www.youtube.com/watch?v=4eeESJQk-mw
///////////////////////////////////////////////////////////////////////////////

class Circle
{
public:
    Circle(double radius) :radius_(radius)
    {}

    double getRadius() const { return radius_; }

private:
    double radius_;
};

class Square
{
public:
    Square(double sideLength) :sideLength_(sideLength)
    {}

    double getSideLength() const { return sideLength_; }

private:
    double sideLength_;
};

void drawAlone(const Circle& circle)
{
    printf("draw a circle, radius: %.1f\n", circle.getRadius());
}

void drawAlone(const Square& square)
{
    printf("draw a square, side length: %.1f\n", square.getSideLength());
}

void serializeAlone(const Circle& circle)
{
    printf("serialize a circle\n");
}

void serializeAlone(const Square& square)
{
    printf("serialize a square\n");
}

class Shape
{
private:
    struct ShapeConcept
    {
        virtual ~ShapeConcept() = default;
        virtual void serialize() const = 0;
        virtual void draw() const = 0;
        virtual std::unique_ptr<ShapeConcept> clone() const = 0;
    };

    template<typename T>
    struct ShapeModel : ShapeConcept
    {
        ShapeModel(T&& value)
            :object{ std::forward<T>(value) }
        {
        }

        std::unique_ptr<ShapeConcept> clone() const override
        {
            return std::make_unique<ShapeModel>(*this);
        }

        void serialize() const override
        {
            serializeAlone(object);
        }

        void draw() const override
        {
            drawAlone(object);
        }

        T object;
    };

    std::unique_ptr<ShapeConcept> pimpl;

public:
    template<typename T>
    Shape(T && x)
        :pimpl{ new ShapeModel<T>(std::forward<T>(x)) }
    {
    }

    void serialize() const
    {
        pimpl->serialize();
    }

    void draw() const
    {
        pimpl->draw();
    }
};

void drawAllShapes(std::vector<Shape> const& shapes)
{
    for (auto const& shape : shapes)
    {
        shape.draw();
    }
}

TEST(type_eraser_shape, demo)
{
    using Shapes = std::vector<Shape>;

    Shapes shapes;
    Circle circle{ 2.0 };
    shapes.emplace_back(circle);
    shapes.emplace_back(Square{ 1.5 });
    shapes.emplace_back(Circle{ 4.3 });

    drawAllShapes(shapes);
}
