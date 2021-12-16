#pragma once
#include "helper.h"

class day16 : public day_base<16>
{
    struct bitstream
    {
        bitstream(const std::string& hex_input) : read_index(0)
        {
            for (auto h : hex_input)
            {
                unsigned char val = (h >= 'A') ? (h - 'A' + 10) : (h - '0');
                for (int i = 3; i>= 0 ; i--)
                {
                    values.push_back((val >> i) & 0x1);
                }
            }
        }

        bitstream(bitstream& orig, size_t number_of_bits) : read_index(0), values(orig.get_part_of_stream(number_of_bits))
        {
        }

        bool is_empty_or_contains_zeros_only() const
        {
            return read_index == values.size() || std::all_of(std::begin(values) + read_index, std::end(values), [](auto v) {return v == 0; });
        }

        size_t getvalue(size_t number_of_bits)
        {
            size_t value = 0;
            for (size_t i = 0; i < number_of_bits; i++)
            {
                value = (value << 1) + values[read_index++];
            }
            return value;
        }

    private:

        std::vector<unsigned char> get_part_of_stream(size_t number_of_bits)
        {
            read_index += number_of_bits;
            return { values.begin() + read_index - number_of_bits, values.begin() + read_index};
        }

        size_t read_index;
        std::vector<unsigned char> values;
    };

    struct packet_base
    {
        packet_base(size_t v) : version(v) {}

        size_t get_version() const
        {
            return version;
        }

        virtual size_t get_sum_of_versions() const = 0;
        virtual int64_t get_value() const = 0;

    private:        

        size_t version;
    };

    struct packet_factory;

    struct literal_packet : public packet_base
    {
        literal_packet(size_t v, bitstream& b) : packet_base(v)
        {
            value = 0;
            size_t binary_value = b.getvalue(5);
            while ((binary_value & 16) > 0)
            {
                value = (value + (binary_value & 15)) << 4;
                binary_value = b.getvalue(5);
            }
            value += (binary_value & 15);
        }

        size_t get_sum_of_versions() const override
        {
            return get_version();
        }

        int64_t get_value () const override
        {
            return value;
        }

    private:
        size_t value;
    };

    
    struct operator_packet : public packet_base
    {
        operator_packet(size_t v, size_t t, bitstream& b) :packet_base(v)
        {
            type = t;
            auto ltid = b.getvalue(1);
            if (ltid == 1)
            {
                size_t number_of_subpackets = b.getvalue(11);
                for (size_t i = 0; i < number_of_subpackets; i++)
                {
                    packets.push_back(packet_factory::create_packet(b));
                }
            }
            else
            {
                bitstream m_bs(b, b.getvalue(15));
                while (!m_bs.is_empty_or_contains_zeros_only())
                {
                    packets.push_back(packet_factory::create_packet(m_bs));
                }
            }
        }

        size_t get_sum_of_versions() const override
        {
            return std::accumulate(std::cbegin(packets), std::cend(packets), get_version(), [](auto a, const auto& b) {return a + b->get_sum_of_versions(); });
        }

        int64_t get_value() const override
        {
            if (type == 0)
            {
                return std::accumulate(std::cbegin(packets), std::cend(packets), static_cast<size_t>(0), [](auto a, const auto& b) {return a + b->get_value(); });
            }
            if (type == 1)
            {
                return std::accumulate(std::cbegin(packets), std::cend(packets), static_cast<size_t>(1), [](auto a, const auto& b) {return a * b->get_value(); });
            }
            if (type == 2)
            {
                return (*std::min_element(std::cbegin(packets), std::cend(packets), [](auto& a, auto& b) { return a->get_value() < b->get_value(); }))->get_value();
            }
            if (type == 3)
            {
                return (*std::max_element(std::cbegin(packets), std::cend(packets), [](auto& a, auto& b) { return a->get_value() < b->get_value(); }))->get_value();
            }
            if (type == 5)
            {
                return (packets[0]->get_value() > packets[1]->get_value()) ? 1 : 0;
            }
            if (type == 6)
            {
                return (packets[0]->get_value() < packets[1]->get_value()) ? 1 : 0;
            }
            if (type == 7)
            {
                return (packets[0]->get_value() == packets[1]->get_value()) ? 1 : 0;
            }
            
            return 0;
        }

    private:

        std::vector<std::unique_ptr<packet_base>> packets;
        size_t type;
    };


    struct packet_factory
    {
        static std::unique_ptr<packet_base> create_packet(bitstream& b)
        {
            auto version = b.getvalue(3);
            auto type = b.getvalue(3);

           if (type == 4)
           {
               return std::make_unique<literal_packet>(version, b);
           }
           else
           {
               return std::make_unique<operator_packet>(version, type, b);
           }
        }
    };

    void run_interal() override
    {
        bitstream b(input_reader().get_line());
        auto root_packet = packet_factory::create_packet(b);
        set_star1_result(root_packet->get_sum_of_versions());
        set_star2_result(root_packet->get_value());
    }
};
