

typedef struct position {
	int first;
	int second;
};

class Piece {
	private: 
		bool m_alive;
		int m_type;
		int m_id;
		bool m_team;
		position m_location;
		bool m_hasMoved;
		int m_weight;

	public:
		Piece();
		Piece(int type, int id, bool team);
		~Piece();
		void kill() { m_alive = false; };
		void setLocation(position location) { m_location = location; };
		void setHasMoved(bool move) {m_hasMoved = move; }
		void setType(int type) { m_type = type; };
		void setWeight(int weight) { m_weight = weight; };

		int getType() { return m_type; };
		int getId() { return m_id; };
		bool getTeam() { return m_team; };
		bool getifAlive() { return m_alive; };
		position getLocation() { return m_location; };
		bool getIfHasMoved() { return m_hasMoved; }
		int getWeight() { return m_weight; }
};