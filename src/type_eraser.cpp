#include <gtest/gtest.h>

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: C++ 'Type Erasure' Explained
// Web: https://davekilian.com/cpp-type-erasure.html
///////////////////////////////////////////////////////////////////////////////

class Cow
{
public:
    const char *see() const { return "cow"; }
    const char *say() const { return "moo"; }
};

class Pig
{
public:
    const char *see() const { return "pig"; }
    const char *say() const { return "oink"; }
};

class Dog
{
public:
    const char *see() const { return "dog"; }
    const char *say() const { return "woof"; }
};

class SeeAndSay
{
    class AnimalConcept
    {
    public:
        virtual ~AnimalConcept() = default;
        virtual const char *see() const = 0;
        virtual const char *say() const = 0;
    };

    template <typename T>
    class AnimalModel : public AnimalConcept
    {
        const T *m_animal;

    public:
        AnimalModel(const T *animal)
            : m_animal(animal)
        { }

        const char *see() const override { return m_animal->see(); }
        const char *say() const override { return m_animal->say(); }
    };

    std::vector<AnimalConcept*> m_animals;

public:
    template <typename T>
    void addAnimal(T *animal)
    {
        m_animals.push_back(new AnimalModel<T>(animal));
    }

    void pullTheString()
    {
        size_t index = rand() % m_animals.size();

        AnimalConcept *animal = m_animals[index];
        printf("The %s says '%s!'\n",
            animal->see(),
            animal->say());
    }
};

TEST(type_eraser, test)
{
    SeeAndSay action;
    action.addAnimal<Cow>(new Cow);
    action.addAnimal<Pig>(new Pig);
    action.addAnimal<Dog>(new Dog);

    action.pullTheString();
}
