-- =================================================================
-- Schema for storing generated project templates
-- =================================================================

-- Drop table if it exists to ensure a clean setup
DROP TABLE IF EXISTS templates;

-- Create the main table for storing template data
CREATE TABLE templates (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_name TEXT NOT NULL,
    file_path TEXT NOT NULL,
    content TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(project_name, file_path)
);

-- =================================================================
-- Indexes for performance
-- =================================================================

-- Index on project_name for quick lookups of all files for a project
CREATE INDEX idx_project_name ON templates(project_name);

-- =================================================================
-- End of Schema
-- =================================================================
