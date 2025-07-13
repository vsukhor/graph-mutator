/* =============================================================================

Copyright (c) 2021-2025 Valerii Sukhorukov <vsukhorukov@yahoo.com>
All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

================================================================================
*/

/**
 * @file vector_container.h
 * @brief Contains definition of a vector container class.
 * @details This class provides a simple wrapper around std::vector to manage
 * elements of a specific type. It provides basic functionalities such as
 * accessing elements, checking size, and modifying the container.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_VECTOR_CONTAINER_H
#define GRAPH_MUTATOR_VECTOR_CONTAINER_H

#include <vector>


namespace graph_mutator {

/**
 * @brief Contains definition of a vector container class.
 * @details This class provides a simple wrapper around std::vector to manage
 * elements of a specific type. It provides basic functionalities such as
 * accessing elements, checking size, and modifying the container.
 * @tparam Type Type of elements stored in the container.
 */
 template<typename Type>
struct VectorContainer {

    using Container = std::vector<Type>;

    using size_type = Container::size_type;
    using value_type = Container::value_type;
    using iterator = Container::iterator;
    using const_iterator = Container::const_iterator;
    using reference = Container::reference;
    using const_reference = Container::const_reference;

    auto operator[](const size_type i) noexcept -> reference;
    auto operator[](const size_type i) const noexcept -> const_reference;

    constexpr auto size() const noexcept;
    constexpr bool empty() const noexcept;
    constexpr void clear() noexcept;

    constexpr auto front() noexcept -> reference;
    constexpr auto front() const noexcept -> const_reference;

    constexpr auto back() noexcept -> reference;
    constexpr auto back() const noexcept -> const_reference;

    constexpr auto begin() noexcept -> iterator;
    constexpr auto begin() const noexcept -> const_iterator;

    constexpr auto end() noexcept -> iterator;
    constexpr auto end() const noexcept -> const_iterator;

    template<class... Args>
    constexpr auto emplace_back(Args&&... args) -> reference;

    constexpr void push_back(const Type& value);
    constexpr void push_back(Type&& value);

    constexpr void pop_back();

    constexpr auto erase(const_iterator pos) -> iterator;
	constexpr auto erase(const_iterator first, const_iterator last) -> iterator;

protected:

    Container data;
};


template<typename Type>
auto VectorContainer<Type>::
operator[](const size_type i) noexcept -> reference
{
    return data[i];
}

template<typename Type>
auto VectorContainer<Type>::
operator[](const size_type i) const noexcept -> const_reference
{
    return data[i];
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
size() const noexcept
{
    return data.size();
}

template<typename Type>
constexpr
bool VectorContainer<Type>::
empty() const noexcept
{
    return data.empty();
}

template<typename Type>
constexpr
void VectorContainer<Type>::
clear() noexcept
{
    return data.clear();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
front() noexcept -> reference
{
    return data.front();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
front() const noexcept -> const_reference
{
    return data.front();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
back() noexcept -> reference
{
    return data.back();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
back() const noexcept -> const_reference
{
    return data.back();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
begin() noexcept -> iterator
{
    return data.begin();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
begin() const noexcept -> const_iterator
{
    return data.begin();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
end() noexcept -> iterator
{
    return data.end();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
end() const noexcept -> const_iterator
{
    return data.end();
}

template<typename Type>
template<class... Args >
constexpr
auto VectorContainer<Type>::
emplace_back(Args&&... args) -> reference
{
    return data.emplace_back(args...);
}

template<typename Type>
constexpr
void VectorContainer<Type>::
push_back(const Type& value)
{
    data.push_back(value);
}

template<typename Type>
constexpr
void VectorContainer<Type>::
push_back(Type&& value)
{
    data.push_back(std::move(value));
}

template<typename Type>
constexpr
void VectorContainer<Type>::
pop_back()
{
    data.pop_back();
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
erase(const_iterator pos) -> iterator
{
    return data.erase(pos);
}

template<typename Type>
constexpr
auto VectorContainer<Type>::
erase(const_iterator first, const_iterator last) -> iterator
{
    return data.erase(first, last);
}


}  // namespace graph_mutator

#endif  // GRAPH_MUTATOR_VECTOR_CONTAINER_H