#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class metro_map
{
    map<string, map<string, int>> station;

public:
    // function for add ,num, contains , remove station as vertex
    void addVertex(string st_name)
    {
        if (station.find(st_name) != station.end())
        {
            return;
        }
        map<string, int> nebr;
        station.insert({st_name, nebr});
    }
    int numofVer()
    {
        return station.size();
    }
    bool containsStation(string st_name)
    {
        return station.find(st_name) != station.end();
    }
    void removeVertex(string st_name)
    {
        if (station.find(st_name) == station.end())
        {
            return;
        }
        map<string, int> nbr = station[st_name];
        vector<string> st_nbr;
        for (auto it : nbr)
        {
            st_nbr.push_back(it.first);
        }
        for (auto it : st_nbr)
        {
            station[it].erase(st_name);
        }
        station.erase(st_name);
    }

    // function for adding ,removing,find num , check present of edges
    void addEdge(string st_name1, string st_name2, int dis)
    {
        if (station[st_name1].find(st_name2) != station[st_name1].end())
            return;

        station[st_name1].insert({st_name2, dis});
        station[st_name2].insert({st_name1, dis});
    }
    void removeEdge(string st_name1, string st_name2)
    {
        if (station.find(st_name1) == station.end() || station.find(st_name2) == station.end())
            return;
        if (station[st_name1].find(st_name2) == station[st_name1].end())
            return;
        station[st_name1].erase(st_name2);
        station[st_name2].erase(st_name1);
    }
    int numOfEdges()
    {
        int num = 0;
        for (auto it : station)
        {
            num += station[it.first].size();
        }
        return num / 2;
    }
    bool containEdge(string st_name1, string st_name2)
    {
        if (station.find(st_name1) == station.end() || station.find(st_name2) == station.end())
            return false;
        if (station[st_name1].find(st_name2) != station[st_name1].end())
            return true;
        return false;
    }

    // show stations
    void show_stations()
    {
        int i = 1;
        cout << "\n\t\tList of metro stations" << endl;
        for (auto it : station)
        {
            cout << i++ << ". " << it.first << endl;
        }
        cout << "...................................................................................." << endl;
    }

    // show metro map
    void show_metro_map()
    {
        cout << "\n\t\t DELHI METRO MAP";
        cout << "\n\t\t.....................\n";
        for (auto it : station)
        {
            cout << it.first << "->" << endl;
            for (auto it2 : it.second)
            {
                cout << "\t" << it2.first << "\tdistance " << it2.second << "km" << endl;
            }
            cout << endl;
        }
        cout << "\t\t................................................" << endl;
    }

    // DFS traversal to know exist of path between src and dest
    bool hasPath(string src, string dest)
    {
        map<string, bool> visited;
        return findPath(src, dest, visited);
    }
    bool findPath(string src, string dest, map<string, bool> &visited)
    {
        if (containEdge(src, dest))
            return true;

        visited[src] = true;
        for (auto it : station[src])
        {
            if (!visited[it.first])
            {
                if (findPath(it.first, dest, visited) == true)
                    return true;
            }
        }
        return false;
    }

    // Dijkstra's Algorithm for shortest distance between src and dest
    int shortestdist(string src, string dest)
    {
        map<string, bool> visited;
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        int dist = INT_MAX;
        pq.push({0, src});

        while (!pq.empty())
        {
            int dis = pq.top().first;
            string curr = pq.top().second;
            pq.pop();
            visited[curr] = true;
            if (curr == dest)
            {
                dist = dis;
                break;
            }
            for (auto it : station[curr])
            {
                if (!visited[it.first])
                {
                    pq.push({it.second + dis, it.first});
                }
            }
        }
        return dist == INT_MAX ? -1 : dist;
    }

    // get shortest path between src and dest (dist wise)
    class station_info
    {
        string station_name;
        string path_cover;
        int min_dist;

    public:
        station_info(string st_name)
        {
            this->station_name = st_name;
        }
        string get_station()
        {
            return station_name;
        }
        void addPath(string covered_path)
        {
            path_cover += (covered_path + " ");
        }
        string PathsoFar()
        {
            return path_cover;
        }
        void addDist(int dist)
        {
            min_dist += dist;
        }
        int Dist_cover()
        {
            return min_dist;
        }
    };
    string route_map(string src, string dest)
    {
        string path = "";
        map<string, int> visited;
        int mini_dist = INT_MAX;
        queue<station_info *> route;
        station_info *src_station = new station_info(src);
        src_station->addPath(src);
        src_station->addDist(0);
        route.push(src_station);

        while (!route.empty())
        {
            station_info *current_station_info = route.front();
            route.pop();
            string current_station = current_station_info->get_station();
            int dist = current_station_info->Dist_cover();
            string path_covered = current_station_info->PathsoFar();
            if (current_station == dest)
            {
                if (dist < mini_dist)
                {
                    path.clear();
                    path += path_covered;
                    mini_dist = dist;
                }
                continue;
            }
            visited[current_station] = true;
            for (auto st_nbr : station[current_station])
            {
                if (!visited[st_nbr.first])
                {
                    station_info *next_station = new station_info(st_nbr.first);
                    next_station->addDist(dist + st_nbr.second);
                    next_station->addPath(path_covered +"->"+ st_nbr.first);
                    route.push(next_station);
                }
            }
        }
        return path;
    }

    // No. of station between src and dest (path is given)(both src and dest is included)
    int Num_station(string path)
    {
        int cnt = 0;
        for (auto it : path)
        {
            if (it == '~')
            {
                cnt++;
            }
        }
        return cnt;
    }
    // No. of interchanges and interchange_station between src and dest
    void interchanges(string path, string &interchange, int &count)
    {
        int cnt = 0;
        string interchange_station = "";
        vector<string> split_path;
        vector<string> split_station;
        string split;
        for (int i = 0; i < path.size(); i++)
        {
            if(path[i]=='-'||path[i]=='>')continue;
            if (path[i] == '~')
            {
                split += path[i];
                i++;
                string line = "";
                while (path[i] != ' ')
                {
                    line += path[i];
                    split += path[i];
                    i++;
                }
                split_path.push_back(line);
                split_station.push_back(split);
                split.clear();
            }
            else
            {  
                split += path[i];
            }
        }
        string line = split_path[0];
        for (int i = 1; i < split_path.size(); i++)
        {
            bool same = 0;
            for (auto it : split_path[i])
            {
                for (auto ch : line)
                {
                    if (it == ch)
                    {
                        same = 1;
                        break;
                    }
                }
                if (same)
                    break;
            }
            if (!same)
            {
                interchange_station += split_station[i - 1] + " ";
                cnt++;
                line = split_path[i];
            }
        }
        count = cnt;
        interchange = interchange_station;
    }
};

// create metro_map
void create_metro_map(metro_map &route)
{
    // adding station as vertices
    route.addVertex("IIT Delhi~M");
    route.addVertex("Dashrath Puri~M");
    route.addVertex("Keshav Puram~R");
    route.addVertex("Akshardham~B");
    route.addVertex("Noida Sector 62~B");
    route.addVertex("Botanical Garden~B");
    route.addVertex("Yamuna Bank~B");
    route.addVertex("Janak Puri West~BO");
    route.addVertex("Dwarka Sector 21~B");
    route.addVertex("Huda City Center~Y");
    route.addVertex("Saket~Y");
    route.addVertex("Rajiv Chowk~BY");
    route.addVertex("Vaishali~B");
    route.addVertex("Moti Nagar~B");

    // ADDING EDGES
    route.addEdge("Dashrath Puri~M", "IIT Delhi~M", 13);
    route.addEdge("Keshav Puram~R", "Kashmere Gate~RYV", 8);
    route.addEdge("IIT Delhi~M", "Kalindi Kunj~M", 16);
    route.addEdge("Akshardham~B", "Rajiv Chowk~BY", 7);
    route.addEdge("Rajiv Chowk~BY", "Janakpuri West~BM",15);
    route.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
    route.addEdge("Botanical Garden~B", "Yamuna Bank~B", 9);
    route.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
    route.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
    route.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
    route.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
    route.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
    route.addEdge("Huda City Center~Y", "Saket~Y", 16);
    route.addEdge("Saket~Y", "AIIMS~Y", 6);
    route.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
    route.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
    route.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
    route.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
    route.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
    route.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
    route.addEdge("DDS Campus~O", "IGI Airport~O", 8);
    route.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
    route.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
    route.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);
    route.addEdge("Adarsh Nagar~Y", "Kashmere Gate~RYV", 8);
    route.addEdge("Kashmere Gate~RYV", "Chandni Chowk~Y", 1);
    route.addEdge("Akshardham~B", "Yamuna Bank~B", 2);
    route.addEdge("Dashrath Puri~M", "Janakpuri West~BM", 4);
    route.addEdge("Mansarovar Park~R", "Keshav Puram~R", 15);
}
int main()
{
    metro_map route;
    create_metro_map(route);
    cout << "\n\t\t\t****Welcome to Delhi Metro route system****\n"
         << "\t\t\t............................................."
         << endl;
    bool exist;
    do
    {
        cout << "\t\t\t\t**How Can I help you?**\n"
             << "\t\t\t\t.........................." << endl;
        cout << "1. List of all stations " << endl;
        cout << "2. Show the Delhi metro route map" << endl;
        cout << "3. To know the minimum distance between source station and destination station" << endl;
        cout << "4. To know the time taken to reach from source station to destination station" << endl;
        cout << "5. To get the path information from source station to destination station" << endl;

        cout << ".................................................................................." << endl;
        cout << "Enter serial number from above options" << endl;
        int n;
        cin >> n;
        switch (n)
        {
        case 1:
        {
            route.show_stations();
            cout << endl;
            cout << "Please Enter`y`/`Y` to use again otherwise `N`/`n`." << endl;
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y')
            {
                exist = false;
            }
            else
            {
                exist = true;
                cout << "You existed." << endl;
            }
            break;
        }
        case 2:
        {
            route.show_metro_map();
            cout << endl;
            cout << "Please Enter`y`/`Y` to use again otherwise `N`/`n`." << endl;
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y')
            {
                exist = false;
            }
            else
            {
                exist = true;
                cout << "You existed." << endl;
            }
            break;
        }
        case 3:
        {
            string src;
            cout << "Enter source station: ";
            getline(cin, src);
            getline(cin, src);
            string dest;
            cout << "Enter destination station: ";
            getline(cin, dest);
            if (!route.containsStation(src) || !route.containsStation(dest) || !route.hasPath(src, dest))
            {
                cout << "The input is invalid. Please Enter inputs again" << endl;
            }
            else
            {
                cout << "The minimum distance between " << src << " station " << dest << " station is "
                     << route.shortestdist(src, dest) << " km" << endl;
            }
            cout << "\t\t.............................................................";
            cout << endl;
            cout << "Please Enter`y`/`Y` to use again otherwise `N`/`n`." << endl;
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y')
            {
                exist = false;
            }
            else
            {
                exist = true;
                cout << "You existed." << endl;
            }
            break;
        }
        case 4:
        {
            string src;
            cout << "Enter source station: ";
            getline(cin, src);
            getline(cin, src);
            string dest;
            cout << "and destination station: ";
            getline(cin, dest);
            if (!route.containsStation(src) || !route.containsStation(dest) || !route.hasPath(src, dest))
            {
                cout << "The input is invalid. Please Enter inputs again" << endl;
            }
            else
            {
                int minidist = route.shortestdist(src, dest);
                int hr = minidist / 45;
                int minutes = (minidist % 45) * 60 / 45;
                cout << "The minimum timetaken to reach from " << src << " station to " << dest << " station is ";
                if (hr == 1)
                    cout << hr << "hr ";
                if (hr > 1)
                    cout << hr << "hrs ";
                if (minutes == 1)
                    cout << minutes << "minute ";
                if (minutes > 1)
                    cout << minutes << "minutes ";
                cout << endl;
            }
            cout << "\t\t.............................................................";
            cout << endl;
            cout << "Please Enter`y`/`Y` to use again otherwise `N`/`n`." << endl;
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y')
            {
                exist = false;
            }
            else
            {
                exist = true;
                cout << "You existed." << endl;
            }
            break;
        }
        case 5:
        {
            string src;
            cout << "Enter source station: ";
            getline(cin, src);
            getline(cin, src);
            string dest;
            cout << "Enter destination station: ";
            getline(cin, dest);
            cout<<endl;
            string str = route.route_map(src, dest);
            string interchange_station = "";
            int No_interchange = 0;
            route.interchanges(str, interchange_station, No_interchange);
            cout << "The path to reach " << dest << " from " << src << " station is :" << endl;
            cout << str << endl;
            cout << "No. of stations: " << route.Num_station(str) << endl;
            cout << "Minimum distance to travel:" << route.shortestdist(src, dest) << " km" << endl;
            cout << "No. of interchanges station: " << No_interchange << endl;
            cout << "Interchanges station: " << interchange_station << endl;
            cout << endl;
            cout << "\t\t.............................................................\n";
            cout << "Please Enter`y`/`Y` to use again otherwise `N`/`n`." << endl;
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y')
            {
                exist = false;
            }
            else
            {
                exist = true;
                cout << "You existed." << endl;
            }
            break;
        }
        default:
        {
            cout << "You  entered an invalid option."
                 << endl;
            cout << "Please Enter`y`/`Y` to use again otherwise `N`/`n`." << endl;
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y')
            {
                exist = false;
            }
            else
            {
                exist = true;
                cout << "You existed." << endl;
            }
            break;
        }
        }
    } while (!exist);
}