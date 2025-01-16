import tkinter as tk
from tkinter import filedialog

SPRITE_TYPES = {
    "BASIC_ENEMY": "sprites/enemy.png",
    "SHOOTER_ENEMY": "sprites/shooter.png",
    "BOSS": "sprites/boss.png",
    "BONUS": "sprites/bonus.png"
}

BACKGROUNDS = {
    "Clouds": "clouds.png",
    "Nexus": "nexus.png",
    "Space": "space.png"
}

MUSIC = {
    "Evangelion": "evangelion.ogg",
    "Mortal Kombat": "mortalKombat.ogg",
    "PvP": "PvP.ogg",
    "Street Fighter": "streetFighter.ogg"
}

class LevelEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("Level Editor")
        self.root.geometry("1280x800")

        self.canvas = tk.Canvas(root, width=1280, height=720, bg="white")
        self.canvas.pack()

        self.play_area = {
            "x1": 0.1,
            "y1": 0.1,
            "x2": 0.9,
            "y2": 0.9,
        }
        self.overflow = 0.2

        self.draw_play_area()

        self.control_frame = tk.Frame(root)
        self.control_frame.pack()

        self.sprite_var = tk.StringVar(value="BASIC_ENEMY")
        self.sprite_menu = tk.OptionMenu(self.control_frame, self.sprite_var, *SPRITE_TYPES.keys())
        self.sprite_menu.pack(side=tk.LEFT)

        self.background_var = tk.StringVar(value="Clouds")
        self.background_menu = tk.OptionMenu(self.control_frame, self.background_var, *BACKGROUNDS.keys())
        self.background_menu.pack(side=tk.LEFT)

        self.music_var = tk.StringVar(value="Street Fighter")
        self.music_menu = tk.OptionMenu(self.control_frame, self.music_var, *MUSIC.keys())
        self.music_menu.pack(side=tk.LEFT)

        self.add_button = tk.Button(self.control_frame, text="Add Sprite", command=self.add_sprite)
        self.add_button.pack(side=tk.LEFT)

        self.save_button = tk.Button(self.control_frame, text="Save Level", command=self.save_level)
        self.save_button.pack(side=tk.LEFT)

        self.wave_button = tk.Button(self.control_frame, text="Add Wave", command=self.add_wave)
        self.wave_button.pack(side=tk.LEFT)

        self.load_button = tk.Button(self.control_frame, text="Load Level", command=self.load_level)
        self.load_button.pack(side=tk.LEFT)

        self.wave_var = tk.StringVar()
        self.wave_selector = tk.OptionMenu(self.control_frame, self.wave_var, "Wave 1", command=self.select_wave)
        self.wave_selector.pack(side=tk.LEFT)

        self.waves = []
        self.current_wave = 1

        self.images = {}

        self.canvas.bind("<Button-1>", self.canvas_click)

        self.selected_sprite = None

        self.load_sprite_images()

        self.waves.append({"wave_id": self.current_wave, "sprites": []})
        self.wave_var.set("Wave 1")

        self.edit_frame = tk.Frame(root)
        self.edit_frame.pack(side=tk.BOTTOM, fill=tk.X)

        self.edit_widgets = {
            "x": self.create_edit_field("X Position (0-1 inside, >1 or <0 outside):", 0.5),
            "y": self.create_edit_field("Y Position (0-1 inside, >1 or <0 outside):", 0.5),
            "speed_x": self.create_edit_field("X Velocity (0-1):", 0),
            "speed_y": self.create_edit_field("Y Velocity (0-1):", 0),
            "health": self.create_edit_field("Health:", 500),
            "bonus_type": self.create_edit_field("Bonus Type (0-3):", 0)
        }

        self.apply_button = tk.Button(self.edit_frame, text="Apply Changes", command=self.apply_changes)
        self.apply_button.pack(side=tk.LEFT)

        self.edit_frame.pack_forget()

    def draw_play_area(self):
        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()

        x1 = self.play_area["x1"] * canvas_width
        y1 = self.play_area["y1"] * canvas_height
        x2 = self.play_area["x2"] * canvas_width
        y2 = self.play_area["y2"] * canvas_height

        self.canvas.create_rectangle(x1, y1, x2, y2, outline="blue", dash=(5, 5), tags="play_area")

    def create_edit_field(self, label, default_value):
        frame = tk.Frame(self.edit_frame)
        frame.pack(side=tk.LEFT, padx=5, pady=5)
        tk.Label(frame, text=label).pack()
        entry = tk.Entry(frame)
        entry.insert(0, str(default_value))
        entry.pack()
        return entry

    def load_sprite_images(self):
        for sprite_type, file_name in SPRITE_TYPES.items():
            try:
                self.images[sprite_type] = tk.PhotoImage(file=file_name)
            except Exception as e:
                print(f"Error loading image {file_name}: {e}")

    def add_sprite(self):
        sprite_type = self.sprite_var.get()
        new_sprite = {
            "type": sprite_type,
            "x": 0.5,
            "y": 0.5,
            "speed_x": 0.0,
            "speed_y": 0.0,
            "health": 500,
            "bonus_type": 0
        }
        self.waves[self.current_wave - 1]["sprites"].append(new_sprite)
        print(f"Added sprite: {new_sprite}")
        self.update_canvas()

    def canvas_click(self, event):
        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()

        play_area_x1 = self.play_area["x1"] * canvas_width
        play_area_x2 = self.play_area["x2"] * canvas_width
        play_area_y1 = self.play_area["y1"] * canvas_height
        play_area_y2 = self.play_area["y2"] * canvas_height

        x_percent = (event.x - play_area_x1) / (play_area_x2 - play_area_x1)
        y_percent = (event.y - play_area_y1) / (play_area_y2 - play_area_y1)

        for sprite in self.waves[self.current_wave - 1]["sprites"]:
            sprite_x = sprite["x"] * (play_area_x2 - play_area_x1) + play_area_x1
            sprite_y = sprite["y"] * (play_area_y2 - play_area_y1) + play_area_y1
            if abs(sprite_x - event.x) < 20 and abs(sprite_y - event.y) < 20:
                self.selected_sprite = sprite
                print(f"Selected sprite: {sprite}")
                self.show_edit_fields()
                return

        if self.waves[self.current_wave - 1]["sprites"]:
            self.waves[self.current_wave - 1]["sprites"][-1]["x"] = x_percent
            self.waves[self.current_wave - 1]["sprites"][-1]["y"] = y_percent
            print(f"Placed sprite at: ({x_percent}, {y_percent})")
            self.update_canvas()

    def show_edit_fields(self):
        if not self.selected_sprite:
            return

        sprite = self.selected_sprite
        self.edit_widgets["x"].delete(0, tk.END)
        self.edit_widgets["x"].insert(0, str(round(sprite["x"], 2)))
        self.edit_widgets["y"].delete(0, tk.END)
        self.edit_widgets["y"].insert(0, str(round(sprite["y"], 2)))
        self.edit_widgets["speed_x"].delete(0, tk.END)
        self.edit_widgets["speed_x"].insert(0, str(round(sprite["speed_x"], 2)))
        self.edit_widgets["speed_y"].delete(0, tk.END)
        self.edit_widgets["speed_y"].insert(0, str(round(sprite["speed_y"], 2)))
        self.edit_widgets["health"].delete(0, tk.END)
        self.edit_widgets["health"].insert(0, str(sprite["health"]))
        self.edit_widgets["bonus_type"].delete(0, tk.END)
        self.edit_widgets["bonus_type"].insert(0, str(sprite["bonus_type"]))

        self.edit_frame.pack(side=tk.BOTTOM, fill=tk.X)

    def apply_changes(self):
        if not self.selected_sprite:
            return

        sprite = self.selected_sprite
        sprite["x"] = round(float(self.edit_widgets["x"].get()), 2)
        sprite["y"] = round(float(self.edit_widgets["y"].get()), 2)
        sprite["speed_x"] = round(float(self.edit_widgets["speed_x"].get()), 2)
        sprite["speed_y"] = round(float(self.edit_widgets["speed_y"].get()), 2)
        sprite["health"] = int(self.edit_widgets["health"].get())
        if sprite["type"] == "BONUS":
            sprite["bonus_type"] = int(self.edit_widgets["bonus_type"].get())

        print(f"Updated sprite: {sprite}")
        self.update_canvas()

    def add_wave(self):
        self.current_wave += 1
        self.waves.append({"wave_id": self.current_wave, "sprites": []})
        self.wave_var.set(f"Wave {self.current_wave}")
        self.update_wave_selector()
        print(f"Added wave: {self.current_wave}")
        self.update_canvas()

    def select_wave(self, wave_name):
        self.current_wave = int(wave_name.split(" ")[1])
        self.update_canvas()

    def update_wave_selector(self):
        menu = self.wave_selector["menu"]
        menu.delete(0, "end")
        for wave in range(1, self.current_wave + 1):
            menu.add_command(label=f"Wave {wave}", command=lambda w=wave: self.select_wave(f"Wave {w}"))

    def save_level(self):
        file_path = filedialog.asksaveasfilename(defaultextension=".txt", filetypes=[("Text Files", "*.txt")])
        if file_path:
            try:
                with open(file_path, "w", encoding='ascii') as file:
                    file.write(f"MUSIC({MUSIC[self.music_var.get()]})\n")
                    file.write(f"BACKGROUND({BACKGROUNDS[self.background_var.get()]})\n\n")

                    file.write("# Level config file\n")
                    file.write("# Available commands:\n")
                    file.write("# WAVE(waveID): Declare a new wave\n")
                    file.write("# BASIC_ENEMY(posX, posY, velocityX, velocityY, Health): Create basic enemy\n")
                    file.write("# SHOOTER_ENEMY(posX, posY, velocityX, velocityY, Health): Create shooter enemy\n")
                    file.write("# BOSS(posX, posY, Health): Create a Boss\n")
                    file.write("# MUSIC(filename)\n")
                    file.write("# BACKGROUND(filename)\n")
                    file.write("# BONUS(posX, posY, type[0-3])\n\n")

                    for wave in self.waves:
                        file.write(f"WAVE({wave['wave_id']})\n")
                        for sprite in wave["sprites"]:
                            sprite_type = sprite["type"]
                            x, y = round(sprite["x"], 2), round(sprite["y"], 2)
                            speed_x, speed_y = round(sprite["speed_x"], 2), round(sprite["speed_y"], 2)
                            if sprite_type == "BONUS":
                                file.write(f"{sprite_type}({x}, {y}, {sprite['bonus_type']})\n")
                            elif sprite_type == "BOSS":
                                file.write(f"{sprite_type}({x}, {y}, {sprite['health']})\n")
                            else:
                                health = sprite["health"]
                                file.write(f"{sprite_type}({x}, {y}, {speed_x}, {speed_y}, {health})\n")
                        file.write("\n")
                print(f"Level saved to: {file_path}")
            except UnicodeEncodeError:
                print("Error: Non-ASCII characters detected. File could not be saved in ASCII format.")

    def load_level(self):
        file_path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt")])
        if file_path:
            self.waves = []
            self.current_wave = 1
            with open(file_path, "r") as file:
                for line in file:
                    line = line.strip()
                    if line.startswith("MUSIC"):
                        music_file = line.split("(")[1].split(")")[0]
                        for name, file_name in MUSIC.items():
                            if file_name == music_file:
                                self.music_var.set(name)
                                break
                    elif line.startswith("BACKGROUND"):
                        background_file = line.split("(")[1].split(")")[0]
                        for name, file_name in BACKGROUNDS.items():
                            if file_name == background_file:
                                self.background_var.set(name)
                                break
                    elif line.startswith("WAVE"):
                        wave_id = int(line.split("(")[1].split(")")[0])
                        current_wave = {"wave_id": wave_id, "sprites": []}
                        self.waves.append(current_wave)
                        self.current_wave = wave_id
                    elif line.startswith("BASIC_ENEMY") or line.startswith("SHOOTER_ENEMY"):
                        parts = line.split("(")[1].split(")")[0].split(",")
                        x, y = round(float(parts[0]), 2), round(float(parts[1]), 2)
                        speed_x, speed_y = round(float(parts[2]), 2), round(float(parts[3]), 2)
                        health = int(parts[4])
                        sprite_type = line.split("(")[0]
                        current_wave["sprites"].append({
                            "type": sprite_type,
                            "x": x,
                            "y": y,
                            "speed_x": speed_x,
                            "speed_y": speed_y,
                            "health": health
                        })
                    elif line.startswith("BOSS"):
                        parts = line.split("(")[1].split(")")[0].split(",")
                        x, y = round(float(parts[0]), 2), round(float(parts[1]), 2)
                        health = int(parts[2])
                        current_wave["sprites"].append({
                            "type": "BOSS",
                            "x": x,
                            "y": y,
                            "health": health
                        })
                    elif line.startswith("BONUS"):
                        parts = line.split("(")[1].split(")")[0].split(",")
                        x, y = round(float(parts[0]), 2), round(float(parts[1]), 2)
                        bonus_type = int(parts[2])
                        current_wave["sprites"].append({
                            "type": "BONUS",
                            "x": x,
                            "y": y,
                            "bonus_type": bonus_type
                        })
            print(f"Loaded level from: {file_path}")
            self.update_wave_selector()
            self.wave_var.set(f"Wave {self.current_wave}")
            self.update_canvas()

    def update_canvas(self):
        self.canvas.delete("all")
        self.draw_play_area()
        if self.waves:
            canvas_width = self.canvas.winfo_width()
            canvas_height = self.canvas.winfo_height()

            play_area_x1 = self.play_area["x1"] * canvas_width
            play_area_x2 = self.play_area["x2"] * canvas_width
            play_area_y1 = self.play_area["y1"] * canvas_height
            play_area_y2 = self.play_area["y2"] * canvas_height

            for sprite in self.waves[self.current_wave - 1]["sprites"]:
                x = sprite["x"] * (play_area_x2 - play_area_x1) + play_area_x1
                y = sprite["y"] * (play_area_y2 - play_area_y1) + play_area_y1
                sprite_type = sprite["type"]
                if sprite_type in self.images:
                    self.canvas.create_image(x, y, image=self.images[sprite_type])
                else:
                    print(f"Image not found for sprite type: {sprite_type}")

if __name__ == "__main__":
    root = tk.Tk()
    editor = LevelEditor(root)
    root.mainloop()
