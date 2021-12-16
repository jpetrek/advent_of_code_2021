#pragma once
#include "helper.h"

class day16 : public day_base<16>
{
    struct bitstream
    {
        bitstream(std::vector<unsigned char> bits) : read_index(0), values(bits)
        {
        }

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

        bool is_empty_or_contains_zeros_only() const
        {
            for (size_t i = read_index; i < values.size(); i++) if (values[i] == 1) return false;
            return true;
        }

        size_t getvalue(size_t number_of_bits)
        {
            size_t ret = 0;
            for (size_t i = 0; i < number_of_bits; i++)
            {
                ret = (ret << 1) + values[read_index++];
            }
            return ret;
        }

        std::vector<unsigned char> get_stream(size_t number_of_bits)
        {
            std::vector<unsigned char> ret;
            for (size_t i = 0; i < number_of_bits; i++)
            {
                ret.push_back(values[read_index++]);
            }
            return ret;
        }
    
    private:
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
            size_t val = b.getvalue(5);
            while ((val & 16) > 0)
            {
                value = (value + (val & 15)) << 4;
                val = b.getvalue(5);
            }
            value += (val & 15);
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
                bitstream m_bs(b.get_stream(b.getvalue(15)));
                while (!m_bs.is_empty_or_contains_zeros_only())
                {
                    packets.push_back(packet_factory::create_packet(m_bs));
                }
            }
        }

        size_t get_sum_of_versions() const override
        {
            size_t ret = get_version();
            for (const auto& ip : packets) ret+=ip->get_sum_of_versions();
            return ret;
        }

        int64_t get_value() const override
        {
            if (type == 0)
            {
                int64_t sum = 0;
                for (const auto& ip : packets) sum += ip->get_value();
                return sum;
            }
            if (type == 1)
            {
                int64_t mult = 1;
                for (const auto& ip : packets) mult *= ip->get_value();
                return mult;
            }
            if (type == 2)
            {
                min_max_counter<int64_t> mm;
                for (const auto& ip : packets) mm.check_value(ip->get_value());
                return mm.minimum();
            }
            if (type == 3)
            {
                min_max_counter<int64_t> mm;
                for (const auto& ip : packets) mm.check_value(ip->get_value());
                return mm.maximum();
            }
            if (type == 5)
            {
                if (packets[0]->get_value() > packets[1]->get_value()) return 1;
                return 0;
            }
            if (type == 6)
            {
                if (packets[0]->get_value() < packets[1]->get_value()) return 1;
                return 0;
            }
            if (type == 7)
            {
                if (packets[0]->get_value() == packets[1]->get_value()) return 1;
                return 0;
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
