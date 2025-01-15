import tkinter as tk
from tkinter import filedialog

# Constants for sprite types
SPRITE_TYPES = {
    "BASIC_ENEMY": "sprites/enemy.png",
    "SHOOTER_ENEMY": "sprites/shooter.png",
    "BOSS": "sprites/boss.png",
    "BONUS": "sprites/bonus.png"
}

class LevelEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("Level Editor")
        self.root.geometry("1280x850")  # Set window size to 1280x720

        # Canvas for placing sprites
        self.canvas = tk.Canvas(root, width=1280, height=720, bg="white")
        self.canvas.pack()

        # Frame for controls
        self.control_frame = tk.Frame(root)
        self.control_frame.pack()

        # Dropdown for sprite selection
        self.sprite_var = tk.StringVar(value="BASIC_ENEMY")
        self.sprite_menu = tk.OptionMenu(self.control_frame, self.sprite_var, *SPRITE_TYPES.keys())
        self.sprite_menu.pack(side=tk.LEFT)

        # Button to add sprite
        self.add_button = tk.Button(self.control_frame, text="Add Sprite", command=self.add_sprite)
        self.add_button.pack(side=tk.LEFT)

        # Button to save level
        self.save_button = tk.Button(self.control_frame, text="Save Level", command=self.save_level)
        self.save_button.pack(side=tk.LEFT)

        # Button to add new wave
        self.wave_button = tk.Button(self.control_frame, text="Add Wave", command=self.add_wave)
        self.wave_button.pack(side=tk.LEFT)

        # Button to load level
        self.load_button = tk.Button(self.control_frame, text="Load Level", command=self.load_level)
        self.load_button.pack(side=tk.LEFT)

        # List to store wave data
        self.waves = []  # Each wave is a dictionary: {"wave_id": int, "sprites": list}

        # Current wave ID
        self.current_wave = 1

        # Dictionary to store PhotoImage objects
        self.images = {}

        # Bind canvas click event
        self.canvas.bind("<Button-1>", self.canvas_click)

        # Selected sprite for editing
        self.selected_sprite = None

        # Load sprite images
        self.load_sprite_images()

        # Initialize with default wave 1
        self.waves.append({"wave_id": self.current_wave, "sprites": []})

        # Frame for sprite editing
        self.edit_frame = tk.Frame(root)
        self.edit_frame.pack(side=tk.BOTTOM, fill=tk.X)

        # Widgets for sprite editing
        self.edit_widgets = {
            "x": self.create_edit_field("X Position (0-1):", 0.5),
            "y": self.create_edit_field("Y Position (0-1):", 0.5),
            "speed_x": self.create_edit_field("X Velocity (0-1):", 0),
            "speed_y": self.create_edit_field("Y Velocity (0-1):", 0),
            "health": self.create_edit_field("Health:", 500),
            "bonus_type": self.create_edit_field("Bonus Type (0-3):", 0)
        }

        # Button to apply changes
        self.apply_button = tk.Button(self.edit_frame, text="Apply Changes", command=self.apply_changes)
        self.apply_button.pack(side=tk.LEFT)

        # Hide edit frame initially
        self.edit_frame.pack_forget()

    def create_edit_field(self, label, default_value):
        """Create a labeled entry field for editing sprite properties."""
        frame = tk.Frame(self.edit_frame)
        frame.pack(side=tk.LEFT, padx=5, pady=5)
        tk.Label(frame, text=label).pack()
        entry = tk.Entry(frame)
        entry.insert(0, str(default_value))
        entry.pack()
        return entry

    def load_sprite_images(self):
        """Load sprite images into memory."""
        for sprite_type, file_name in SPRITE_TYPES.items():
            try:
                self.images[sprite_type] = tk.PhotoImage(file=file_name)
            except Exception as e:
                print(f"Error loading image {file_name}: {e}")

    def add_sprite(self):
        """Add a sprite to the current wave."""
        sprite_type = self.sprite_var.get()
        new_sprite = {
            "type": sprite_type,
            "x": 0.5,  # Default position (center of the canvas)
            "y": 0.5,
            "speed_x": 0.0,  # Default velocity (0% of canvas width/height per second)
            "speed_y": 0.0,
            "health": 500,
            "bonus_type": 0  # Default bonus type
        }
        self.waves[-1]["sprites"].append(new_sprite)  # Add sprite to the current wave
        print(f"Added sprite: {new_sprite}")
        self.update_canvas()

    def canvas_click(self, event):
        """Handle canvas click to place or select sprites."""
        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()
        x_percent = event.x / canvas_width
        y_percent = event.y / canvas_height

        # Check if a sprite is clicked
        for sprite in self.waves[-1]["sprites"]:  # Only check sprites in the current wave
            sprite_x = sprite["x"] * canvas_width
            sprite_y = sprite["y"] * canvas_height
            if abs(sprite_x - event.x) < 20 and abs(sprite_y - event.y) < 20:  # Check if click is near a sprite
                self.selected_sprite = sprite
                print(f"Selected sprite: {sprite}")
                self.show_edit_fields()
                return

        # If no sprite is clicked, place a new sprite
        if self.waves[-1]["sprites"]:
            self.waves[-1]["sprites"][-1]["x"] = x_percent
            self.waves[-1]["sprites"][-1]["y"] = y_percent
            print(f"Placed sprite at: ({x_percent}, {y_percent})")
            self.update_canvas()

    def show_edit_fields(self):
        """Show the edit fields for the selected sprite."""
        if not self.selected_sprite:
            return

        sprite = self.selected_sprite
        self.edit_widgets["x"].delete(0, tk.END)
        self.edit_widgets["x"].insert(0, str(sprite["x"]))
        self.edit_widgets["y"].delete(0, tk.END)
        self.edit_widgets["y"].insert(0, str(sprite["y"]))
        self.edit_widgets["speed_x"].delete(0, tk.END)
        self.edit_widgets["speed_x"].insert(0, str(sprite["speed_x"]))
        self.edit_widgets["speed_y"].delete(0, tk.END)
        self.edit_widgets["speed_y"].insert(0, str(sprite["speed_y"]))
        self.edit_widgets["health"].delete(0, tk.END)
        self.edit_widgets["health"].insert(0, str(sprite["health"]))
        self.edit_widgets["bonus_type"].delete(0, tk.END)
        self.edit_widgets["bonus_type"].insert(0, str(sprite["bonus_type"]))

        # Show the edit frame
        self.edit_frame.pack(side=tk.BOTTOM, fill=tk.X)

    def apply_changes(self):
        """Apply changes from the edit fields to the selected sprite."""
        if not self.selected_sprite:
            return

        sprite = self.selected_sprite
        sprite["x"] = float(self.edit_widgets["x"].get())
        sprite["y"] = float(self.edit_widgets["y"].get())
        sprite["speed_x"] = float(self.edit_widgets["speed_x"].get())
        sprite["speed_y"] = float(self.edit_widgets["speed_y"].get())
        sprite["health"] = int(self.edit_widgets["health"].get())
        if sprite["type"] == "BONUS":
            sprite["bonus_type"] = int(self.edit_widgets["bonus_type"].get())

        print(f"Updated sprite: {sprite}")
        self.update_canvas()

    def add_wave(self):
        """Add a new wave to the level data."""
        self.current_wave += 1
        self.waves.append({"wave_id": self.current_wave, "sprites": []})  # Add a new wave with an empty sprite list
        print(f"Added wave: {self.current_wave}")
        self.update_canvas()

    def save_level(self):
        """Save the level data to a file with the updated format."""
        file_path = filedialog.asksaveasfilename(defaultextension=".txt", filetypes=[("Text Files", "*.txt")])
        if file_path:
            with open(file_path, "w") as file:
                file.write("# Level config file\n")
                file.write("# Available commands:\n")
                file.write("# WAVE(waveID): Declare a new wave\n")
                file.write("# BASIC_ENEMY(posX, posY, velocityX, velocityY, Health): Create basic enemy\n")
                file.write("# SHOOTER_ENEMY(posX, posY, velocityX, velocityY, Health): Create shooter enemy\n")
                file.write("# BOSS(posX, posY, Health): Create a Boss\n")
                file.write("# MUSIC(filename)\n")
                file.write("# BACKGROUND(filename)\n")
                file.write("# BONUS(posX, posY, type[0-3])\n\n")
                file.write("MUSIC(streetFighter.ogg)\n")
                file.write("BACKGROUND(clouds.jpg)\n\n")

                for wave in self.waves:
                    file.write(f"WAVE({wave['wave_id']})\n")
                    for sprite in wave["sprites"]:
                        sprite_type = sprite["type"]
                        x, y = sprite["x"], sprite["y"]
                        speed_x, speed_y = sprite["speed_x"], sprite["speed_y"]
                        if sprite_type == "BONUS":
                            file.write(f"{sprite_type}({x}, {y}, {sprite['bonus_type']})\n")
                        elif sprite_type == "BOSS":
                            file.write(f"{sprite_type}({x}, {y}, {sprite['health']})\n")
                        else:
                            health = sprite["health"]
                            file.write(f"{sprite_type}({x}, {y}, {speed_x}, {speed_y}, {health})\n")
                    file.write("\n")
            print(f"Level saved to: {file_path}")

    def load_level(self):
        """Load an existing level file."""
        file_path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt")])
        if file_path:
            self.waves = []
            self.current_wave = 1
            with open(file_path, "r") as file:
                current_wave = None
                for line in file:
                    line = line.strip()
                    if line.startswith("WAVE"):
                        wave_id = int(line.split("(")[1].split(")")[0])
                        current_wave = {"wave_id": wave_id, "sprites": []}
                        self.waves.append(current_wave)
                        self.current_wave = wave_id
                    elif line.startswith("BASIC_ENEMY") or line.startswith("SHOOTER_ENEMY"):
                        parts = line.split("(")[1].split(")")[0].split(",")
                        x, y = float(parts[0]), float(parts[1])
                        speed_x, speed_y = float(parts[2]), float(parts[3])
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
                        x, y = float(parts[0]), float(parts[1])
                        health = int(parts[2])
                        current_wave["sprites"].append({
                            "type": "BOSS",
                            "x": x,
                            "y": y,
                            "health": health
                        })
                    elif line.startswith("BONUS"):
                        parts = line.split("(")[1].split(")")[0].split(",")
                        x, y = float(parts[0]), float(parts[1])
                        bonus_type = int(parts[2])
                        current_wave["sprites"].append({
                            "type": "BONUS",
                            "x": x,
                            "y": y,
                            "bonus_type": bonus_type
                        })
            print(f"Loaded level from: {file_path}")
            self.update_canvas()

    def update_canvas(self):
        """Update the canvas with the current sprites."""
        self.canvas.delete("all")
        if self.waves:
            canvas_width = self.canvas.winfo_width()
            canvas_height = self.canvas.winfo_height()
            for sprite in self.waves[-1]["sprites"]:  # Only display sprites from the current wave
                x = sprite["x"] * canvas_width
                y = sprite["y"] * canvas_height
                sprite_type = sprite["type"]
                if sprite_type in self.images:
                    self.canvas.create_image(x, y, image=self.images[sprite_type])
                else:
                    print(f"Image not found for sprite type: {sprite_type}")

if __name__ == "__main__":
    root = tk.Tk()
    editor = LevelEditor(root)
    root.mainloop()
