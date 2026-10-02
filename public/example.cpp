namespace {
    template<integral T>
    constexpr auto
        width{numeric_limits<T>::is_signed + numeric_limits<T>::digits};

    template<signed_integral T>
    constexpr T isSameSign(const T x, const T y) noexcept {
        return (x ^ y) >= T{0};
    }

    template<integral T>
    constexpr T ceilDiv(const T x, const T y) noexcept {
        if constexpr (signed_integral<T>) {
            const auto [quot, rem]{div(x, y)};
            return quot + (isSameSign(x, y) && rem != T{0});
        }

        return x / y + (x % y != T{0});
    }

    template<integral T>
    constexpr bool isOdd(const T value) noexcept {
        return bool(value & T{1});
    }

    enum class RadixSortResult : bool {
        IN_INPUT = false,
        IN_OUTPUT = true
    };

    class CanonicalRadixStrategy final {
    private:
        const size_t maxStepM{12};

    public:
        constexpr CanonicalRadixStrategy() noexcept = default;

        constexpr explicit CanonicalRadixStrategy(const size_t maxStep) noexcept
            : maxStepM{maxStep} {}

        constexpr size_t operator()(
            const integral auto length,
            const size_t bits
        ) const noexcept {
            if (length <= 1) [[unlikely]]
                return min(2UZ, bits);

            const auto ilog2{bit_width(length) - 1};
            if (bits <= ilog2)
                return bits;

            const auto step{min(maxStepM, ilog2)};
            return ceilDiv(bits, ceilDiv(bits, step));
        }
    };

    class DefaultRadixStrategy final {
    private:
        const size_t maxStepM{12};

    public:
        constexpr DefaultRadixStrategy() noexcept = default;

        constexpr explicit DefaultRadixStrategy(const size_t maxStep) noexcept
            : maxStepM{maxStep} {}

        constexpr size_t operator()(
            const integral auto,
            const size_t bits
        ) const noexcept {
            return ceilDiv(bits, ceilDiv(bits, maxStepM));
        }
    };

    template<unsigned_integral Size = size_t, class Allocator = allocator<Size>>
    class CountingSorter final {
    private:
        vector<Size, Allocator> count{};

    public:
        constexpr size_t capacity() const noexcept {
            return count.capacity();
        }

        template<
            bidirectional_iterator InIter,
            random_access_iterator OutIter,
            class Ordinalizer
        >
        constexpr void operator()(
            const InIter inFirst,
            const InIter inLast,
            const OutIter outFirst,
            const Ordinalizer &ordinalizer
        ) noexcept {
            using ranges::next;
            using Difference = iterator_traits<InIter>::difference_type;

            if (next(inFirst, 1, inLast) == inLast) [[unlikely]]
                return;

            const auto cardinality{ordinalizer.cardinality()},
                length{size(count)};
            count.resize(cardinality);
            const auto countFirst{begin(count)}, countLast{end(count)};

            fill_n(countFirst, min(length, cardinality), 0UZ);

            for_each(inFirst, inLast,
                [this, &ordinalizer](
                    const auto &value
                ) constexpr noexcept -> void {
                    const auto idx{ordinalizer(value)};
                    ++count[idx];
                }
            );

            inclusive_scan(countFirst, countLast, countFirst);

            for_each(
                make_reverse_iterator(inLast),
                make_reverse_iterator(inFirst),
                [this, outFirst, &ordinalizer](
                    auto &value
                ) constexpr noexcept -> void {
                    const auto idx{ordinalizer(value)};
                    const auto pos{--count[idx]};
                    outFirst[Difference(pos)] = move(value);
                }
            );
        }

        constexpr void reserve(const size_t capacity) noexcept {
            count.reserve(capacity);
        }

        constexpr void shrink_to_fit() noexcept {
            count.shrink_to_fit();
        }
    };

    template<integral T>
    class IntegralDigitizer final {
    private:
        size_t bitsM{width<T>};

    public:
        using ValueType = T;

        constexpr IntegralDigitizer() noexcept = default;

        constexpr explicit IntegralDigitizer(const size_t bits) noexcept
            : bitsM{bits} {}

        constexpr size_t operator()(
            const ValueType value,
            const size_t idx,
            const size_t step
        ) const noexcept {
            const auto result{
                size_t(value >> idx & ((ValueType{1} << step) - ValueType{1}))
            };
            if constexpr (signed_integral<ValueType>)
                if (idx + step >= bitsM)
                    return result ^ size_t(ValueType{1} << (step - 1));

            return result;
        }

        constexpr size_t bits() const noexcept {
            return bitsM;
        }
    };

    template<class Digitizer>
    class Ordinalizer final {
    private:
        const size_t idxM, stepM;
        const Digitizer &digitizerM;

    public:
        using ValueType = Digitizer::ValueType;

        constexpr Ordinalizer(
            const size_t idx,
            const size_t step,
            const Digitizer &digitizer
        ) noexcept : idxM{idx}, stepM{step}, digitizerM{digitizer} {}

        constexpr size_t operator()(const ValueType &value) const noexcept {
            return digitizerM(value, idxM, stepM);
        }

        constexpr size_t cardinality() const noexcept {
            return 1UZ << stepM;
        }
    };

    template<
        random_access_iterator InIter,
        random_access_iterator OutIter,
        class Digitizer,
        class RadixStrategy = DefaultRadixStrategy,
        class Sorter = CountingSorter<>
    >
    static constexpr RadixSortResult radixSort(
        const InIter inFirst,
        const InIter inLast,
        const OutIter outFirst,
        const Digitizer &digitizer,
        const RadixStrategy &strategy = DefaultRadixStrategy{},
        Sorter &&sorter = Sorter{}
    ) noexcept {
        using enum RadixSortResult;

        const auto length{distance(inFirst, inLast)};
        if (length <= 1) [[unlikely]]
            return IN_INPUT;

        const auto outLast{next(outFirst, length)};
        const auto bits{digitizer.bits()}, maxStep{strategy(length, bits)};

        auto idx{0UZ};
        while (idx < bits) {
            const auto step{min(maxStep, bits - idx)};
            const Ordinalizer<Digitizer> ordinalizer{idx, step, digitizer};

            if (isOdd(idx / maxStep))
                sorter(outFirst, outLast, inFirst, ordinalizer);
            else
                sorter(inFirst, inLast, outFirst, ordinalizer);

            idx += step;
        }

        return RadixSortResult{isOdd(ceilDiv(bits, maxStep))};
    }
}

class Solution final {
public:
    constexpr vector<int> sortArray(vector<int> &nums) const noexcept {
        using enum RadixSortResult;

        vector<int> buffer(size(nums));
        const auto result{radixSort(
            begin(nums),
            end(nums),
            begin(buffer),
            IntegralDigitizer<int>{17}
        )};
        if (result == IN_OUTPUT)
            swap(nums, buffer);

        return move(nums);
    }
};