#pragma once

#include <tesla.hpp>
#include <string>

#include <FileSelect.hpp>
#include <TextReader.hpp>
#include <Utils.hpp>

class FileSelectEntry {
public:
    FileSelectEntry(std::string const &path, bool favorite)
        : m_path(path), m_favorite(favorite) {}

    virtual ~FileSelectEntry() {}

    std::string getPath() const { return m_path; }

    std::string label() const {
        return (isFavorite() ? "\u2605 " : "") + getName();
    }

    void toggleFavorite();
    bool isFavorite() const { return m_favorite; }

    bool operator<(FileSelectEntry const &other) const;

    virtual std::string getName() const = 0;
    virtual void select() = 0;

protected:
    std::string const& getPathInternal() const { return m_path; }
    virtual int ordering() const = 0;

    static std::string baseName(std::string path) {
        while (path.size() > 1 && path.back() == '/')
            path.pop_back();
        const size_t separator = path.find_last_of('/');
        return separator == std::string::npos ? path : path.substr(separator + 1);
    }

private:
    std::string m_path;
    bool m_favorite;
};

class FileSelectDirEntry : public FileSelectEntry {
public:
    FileSelectDirEntry(std::string const &path, bool favorite)
        : FileSelectEntry(path, favorite) {}

    std::string getName() const override {
        return baseName(getPathInternal()) + "/";
    }

    void select() override {
        tsl::changeTo<FileSelect>(getPath());
    }

protected:
    int ordering() const override { return 1; }
};

class FileSelectFileEntry : public FileSelectEntry {
public:
    FileSelectFileEntry(std::string const &path, bool favorite)
        : FileSelectEntry(path, favorite) {}

    std::string getName() const override {
        return baseName(getPathInternal());
    }

    void select() override {
        tsl::changeTo<TextReader>(getPath());
    }

protected:
    int ordering() const override { return 2; }
};