#include <iostream>
#include <string>
#include <map>
#include <queue>

using namespace std;

struct Data {
	Data(size_t _id, long long _time, size_t _count) : client_id{ _id }, time{ _time }, count{ _count } {}

	size_t client_id;
	long long time;
	size_t count;
};

class Booking {
private:
	size_t room_count = 0;
	long long current_time = -1'000000'000000'000000; // время последнего события BOOK
	map<size_t, size_t> clients_rooms;
	queue<Data> data;

	void Patch(long long time) {
		current_time = time;

		while (!data.empty() && data.front().time <= current_time - 86400) {       // 86400 — количество секунд в сутках
			if (clients_rooms[data.front().client_id] == 1) {
				clients_rooms.erase(data.front().client_id);
			}
			else {
				--clients_rooms.at(data.front().client_id);
			}

			room_count -= data.front().count;
			data.pop();
		}
	}
public:
	Booking() = default;

	size_t Clients(long long time) {
		Patch(time);
		return clients_rooms.size();
	}

	size_t Rooms(long long time) {
		Patch(time);
		return room_count;
	}

	void Book(Data& d) {
		++clients_rooms[d.client_id];
		data.push(d);
		room_count += d.count;

		Patch(d.time);
	}
};

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	size_t queries;
	map<string, Booking> booking;
	long long current_time = -1'000000'000000'000000;

	cin >> queries;

	while(queries) {
		string command;
		cin >> command;

		if (command == "BOOK") {
			long long time;
			string hotel_name;
			size_t client_id, room_count;
			cin >> time >> hotel_name >> client_id >> room_count;
			
			current_time = time;
			Data d(client_id, time, room_count);
			booking[hotel_name].Book(d);
		}
		else {
			string hotel_name;
			size_t res;
			cin >> hotel_name;
			
			if (command == "CLIENTS") {
				res = booking[hotel_name].Clients(current_time);
			}
			else if(command == "ROOMS") {
				res = booking[hotel_name].Rooms(current_time);
			}
			cout << res << '\n';
		}

		--queries;
	}

	return 0;
}
